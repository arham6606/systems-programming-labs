#include "MMU.hpp"

MMU::MMU(size_t num_virtual_pages, size_t num_frames, size_t page_size,
         size_t tlb_entries)
    : page_table(num_virtual_pages), physical_memory(num_frames, page_size),
      tlb(tlb_entries) {}

uint8_t MMU::extractVpn(uint16_t va) { return (va >> 8) & 0x00FF; }

uint8_t MMU::extractVpo(uint16_t va) { return (va & 0x00FF); }

uint16_t MMU::constructPhysicalAddress(uint8_t frame, uint8_t offset) {
  return ((static_cast<uint16_t>(frame) << 8)) | offset;
}

bool MMU::translate(uint16_t va, uint16_t &pa, bool calculate) const {
  uint8_t vpn = extractVpn(va);
  uint8_t vpo = extractVpo(va);

  auto tlb_result = tlb.lookUp(vpn);
  if (tlb_result.has_value()) {
    if (calculate == true) {
      tlb_hits++;
    
    }
    uint8_t frame = tlb_result.value();
    pa = constructPhysicalAddress(frame, vpo);
    return true;
  }
  if (calculate == true) {
    tlb_misses++;
  }

  auto pt_result = page_table.lookUp(vpn);
  if (!pt_result.has_value()) {
    return false;
  }

  uint8_t frame = pt_result.value();

  if (calculate == true) {
    tlb.insert(vpn, frame);
  }

  pa = constructPhysicalAddress(frame, vpo);
  return true;
}

bool MMU::read(uint16_t va, uint8_t &value, bool calculate) const {
  uint16_t pa;
  if (!translate(va, pa, calculate)) {
    // std::cout << "Page Fault" << std::endl;
    return false;
  }

  uint8_t frame = (pa >> 8) & 0x00FF;
  uint8_t offset = pa & 0x00FF;

  value = physical_memory.read(frame, offset);
  return true;
}

bool MMU::write(uint16_t va, uint8_t value, bool calculate) {
  uint16_t pa;
  if (!translate(va, pa, calculate)) {
    // std::cout << "Page Fault" << std::endl;
    return false;
  }

  uint8_t frame = (pa >> 8) & 0x00FF;
  uint8_t offset = pa & 0x00FF;

  physical_memory.write(frame, offset, value);
  return true;
}

size_t MMU::getTlbHits() const { return tlb_hits; }

size_t MMU::getTlbMisses() const { return tlb_misses; }

double MMU::getTlbHitRate() const {
  size_t total = tlb_hits + tlb_misses;

  if (total == 0) {
    return 0.0;
  }

  double result = (static_cast<double>(tlb_hits) / total) * 100.0;

  return result;
}

#include "MMU.hpp"

MMU::MMU(size_t num_virtual_pages, size_t num_frames, size_t page_size)
    : page_table(num_virtual_pages), physical_memory(num_frames, page_size) {}

uint8_t MMU::extractVpn(uint16_t va) { return (va >> 8) & 0x00FF; }

uint8_t MMU::extractVpo(uint16_t va) { return (va & 0x00FF); }

uint16_t MMU::constructPhysicalAddress(uint8_t frame, uint8_t offset) {
  return ((static_cast<uint16_t>(frame) << 8)) | offset;
}

bool MMU::translate(uint16_t va, uint16_t &pa) const {
  uint8_t vpn = extractVpn(va);
  uint8_t vpo = extractVpo(va);

  auto frame_opt = page_table.lookUp(vpn);
  if (!frame_opt.has_value()) {
    //std::cout << "Page Fauld" << std::endl;
    return false;
  }

  uint8_t frame = frame_opt.value();
  pa = constructPhysicalAddress(frame, vpo);
  return true;
}

bool MMU::read(uint16_t va, uint8_t &value) const {
  uint16_t pa;
  if (!translate(va, pa)) {
   // std::cout << "Page Fault" << std::endl;
    return false;
  }

  uint8_t frame = (pa >> 8) & 0x00FF;
  uint8_t offset = pa & 0x00FF;

  value = physical_memory.read(frame, offset);
  return true;
}

bool MMU::write(uint16_t va, uint8_t value) {
  uint16_t pa;
  if (!translate(va, pa)) {
   // std::cout << "Page Fault" << std::endl;
    return false;
  }

  uint8_t frame = (pa >> 8) & 0x00FF;
  uint8_t offset = pa & 0x00FF;

  physical_memory.write(frame, offset, value);
  return true;
}
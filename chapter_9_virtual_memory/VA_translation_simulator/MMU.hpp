#pragma once
#include "TLB.hpp"
#include "page_table.hpp"
#include "physical_memory.hpp"

class MMU {
private:
  PageTable page_table;
  PhysicalMemory physical_memory;
  mutable TLB tlb;

  mutable size_t tlb_hits = 0;
  mutable size_t tlb_misses = 0;

  static uint8_t extractVpn(uint16_t va);
  static uint8_t extractVpo(uint16_t va);
  static uint16_t constructPhysicalAddress(uint8_t frame, uint8_t offset);

public:
  MMU(size_t num_virtual_pages, size_t num_frames, size_t page_size,
      size_t tlb_entries);

  bool translate(uint16_t va, uint16_t &pa, bool calculate) const;

  bool read(uint16_t va, uint8_t &value, bool calculate) const;

  bool write(uint16_t va, uint8_t value, bool calculate);

  size_t getTlbHits() const;

  size_t getTlbMisses() const;

  double getTlbHitRate() const;

  PageTable &getPageTable() { return page_table; }
};

#pragma once
#include "page_table.hpp"
#include "physical_memory.hpp"

class MMU {
private:
  PageTable page_table;
  PhysicalMemory physical_memory;

  static uint8_t extractVpn(uint16_t va);
  static uint8_t extractVpo(uint16_t va);
  static uint16_t constructPhysicalAddress(uint8_t frame, uint8_t offset);

public:
  MMU(size_t num_virtual_pages, size_t num_frames, size_t page_size);

  bool translate(uint16_t va, uint16_t &pa) const;

  bool read(uint16_t va, uint8_t &value) const;

  bool write(uint16_t va, uint8_t value);

  PageTable &getPageTable() { return page_table; }
};

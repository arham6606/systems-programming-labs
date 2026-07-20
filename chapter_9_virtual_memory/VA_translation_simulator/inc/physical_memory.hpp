#pragma once
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

class PhysicalMemory {
private:
  std::vector<std::vector<uint8_t>> memory;
  size_t page_size;

public:
  PhysicalMemory(size_t num_frames, size_t page_size);

  uint8_t read(size_t frame, size_t byte_offset) const;

  void write(size_t frame, size_t byte_offset, uint8_t value);

  size_t getNumberOfFrames() const;

  size_t getPageSize() const;
};
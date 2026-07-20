#include "physical_memory.hpp"

PhysicalMemory::PhysicalMemory(size_t num_frames, size_t page_size)
    : memory(num_frames, std::vector<uint8_t>(page_size, 0)),
      page_size(page_size) {}

uint8_t PhysicalMemory::read(size_t frame, size_t byte_offset) const {
  if (frame >= memory.size() || byte_offset >= page_size) {
    throw std::out_of_range("Invalid frame or byte offset");
  }
  return memory[frame][byte_offset];
}

void PhysicalMemory::write(size_t frame, size_t byte_offset, uint8_t value) {
  if (frame >= memory.size() || byte_offset >= page_size) {
    throw std::out_of_range("Invalid frame or byte offset");
  }
  memory[frame][byte_offset] = value;
}

size_t PhysicalMemory::getNumberOfFrames() const { return memory.size(); }

size_t PhysicalMemory::getPageSize() const { return page_size; }
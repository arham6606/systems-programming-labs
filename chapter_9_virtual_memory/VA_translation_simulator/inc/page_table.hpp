#pragma once
#include <cstdint>
#include <optional>
#include <vector>

struct PageTableEntry {
  uint8_t frame_number = 0;
  bool valid = false;
};

class PageTable {
private:
  std::vector<PageTableEntry> entries;

public:
  explicit PageTable(size_t num_entries);

  void map(uint8_t virtual_page_number, uint8_t frame_number);

  std::optional<uint8_t> lookUp(uint8_t virtual_page_number) const;

  void invalidate(uint8_t virtual_page_number);

  size_t size() const;
};

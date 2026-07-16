#include "page_table.hpp"

PageTable::PageTable(size_t num_entries) { entries.resize(num_entries); }

void PageTable::map(uint8_t virtual_page_number, uint8_t frame_number) {
  entries[virtual_page_number] = {frame_number, true};
}

std::optional<uint8_t> PageTable::lookUp(uint8_t virtual_page_number) const {
  const auto &entry = entries[virtual_page_number];
  if (entry.valid) {
    return entry.frame_number;
  } else {
    return std::nullopt;
  }
}

void PageTable::invalidate(uint8_t virtual_page_number) {
  entries[virtual_page_number].valid = false;
}

size_t PageTable::size() const { return entries.size(); }


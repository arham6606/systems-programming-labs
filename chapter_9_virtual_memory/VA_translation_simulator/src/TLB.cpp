#include "TLB.hpp"

TLB::TLB(size_t num_entries) { entries.resize(num_entries, {0, 0, false}); }

std::optional<uint8_t> TLB::lookUp(const uint8_t vpn) const {
  for (const auto &entry : entries) {
    if (entry.valid && entry.vpn == vpn) {
      return entry.frame;
    }
  }

  return std::nullopt;
}

void TLB::insert(const uint8_t vpn, const uint8_t frame) {
  for (auto &entry : entries) {
    if (!entry.valid) {
      entry = {vpn, frame, true};
      return;
    }

    if (entry.vpn == vpn) {
      entry.frame = frame;
      return;
    }
  }
  entries[0] = {vpn, frame, true};
}

void TLB::flush() {
  for (auto &entry : entries) {
    entry.valid = false;
  }
}

size_t TLB::getSize() const { return entries.size(); }

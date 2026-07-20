#pragma once
#include <optional>
#include <stdint.h>
#include <vector>

struct TlbEntry {
  uint8_t vpn;
  uint8_t frame;
  bool valid;
};

class TLB {
private:
  std::vector<TlbEntry> entries;

public:
  explicit TLB(size_t num_entries);

  std::optional<uint8_t> lookUp(const uint8_t vpn) const;

  void insert(const uint8_t vpn, const uint8_t frame);

  void flush();

  size_t getSize() const;
};
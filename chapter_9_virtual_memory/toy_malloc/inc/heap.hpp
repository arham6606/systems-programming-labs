#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>

namespace CustomHeap {

struct BlockHeader {
  size_t size;
  bool allocated;
};

class Heap {
private:
  alignas(std::max_align_t) std::byte heap[1024];

  BlockHeader *block;

public:
  Heap() = default;

  std::byte *heapBegin();

  std::byte *heapEnd();

  std::size_t heapSize();

  std::byte *pointerOfPayloadSectionToUser(BlockHeader *header);

  BlockHeader *pointerToHeaderSection(std::byte *user);
};
}; // namespace CustomHeap
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
  alignas(std::max_align_t) std::byte heap[2048];

  BlockHeader *block;

  std::byte *heapBegin();

  std::byte *heapEnd();

  std::byte *pointerOfPayloadSectionToUser(BlockHeader *header);

  BlockHeader *pointerToHeaderSection(std::byte *user);

  BlockHeader *nextBlock(BlockHeader *current);

  BlockHeader *findFreeBlock(std::size_t requested);

  void coalescingFreeBlocks();

public:
  Heap() = default;

  std::size_t heapSize();

  void printHeap();

  void heapInit();

  std::byte *allocate(std::size_t current);

  void deallocate(std::byte *current);

  void printHeapSummary();
};
}; // namespace CustomHeap
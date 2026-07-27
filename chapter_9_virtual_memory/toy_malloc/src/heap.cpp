#include "heap.hpp"

namespace CustomHeap {

std::byte *Heap::heapBegin() { return heap; }

std::byte *Heap::heapEnd() { return heap + sizeof(heap); }

std::size_t Heap::heapSize() { return sizeof(heap); }

std::byte *Heap::pointerOfPayloadSectionToUser(BlockHeader *header) {
  return reinterpret_cast<std::byte *>(header) + sizeof(BlockHeader);
}

BlockHeader *Heap::pointerToHeaderSection(std::byte *user) {
  return reinterpret_cast<BlockHeader *>(user - sizeof(BlockHeader));
}

BlockHeader *Heap::nextBlock(BlockHeader *current) {
  if (current == nullptr) {
    return nullptr;
  }

  std::byte *next = pointerOfPayloadSectionToUser(current) + current->size;

  if (next >= heapEnd()) {
    return nullptr;
  }

  return reinterpret_cast<BlockHeader *>(next);
}

BlockHeader *Heap::findFreeBlock(std::size_t requested) {
  for (BlockHeader *curr = reinterpret_cast<BlockHeader *>(heapBegin());
       curr != nullptr; curr = nextBlock(curr)) {
    if (!curr->allocated && curr->size >= requested) {
      return curr; // First fit found
    }
  }
  return nullptr; // No block large enough
}

}; // namespace CustomHeap
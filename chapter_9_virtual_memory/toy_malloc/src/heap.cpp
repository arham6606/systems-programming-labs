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

std::byte *Heap::allocate(size_t current) {
  BlockHeader *block = findFreeBlock(current);
  if (block == nullptr) {
    return nullptr; // Out of memory
  }
  block->allocated = true;
  // NO SPLITTING
  return pointerOfPayloadSectionToUser(block);
}

void Heap::printHeap() {
  std::cout << "--------------------------------" << std::endl;
  int index = 0;
  for (BlockHeader *curr = reinterpret_cast<BlockHeader *>(heapBegin());
       curr != nullptr; curr = nextBlock(curr)) {

    std::cout << "Block" << index << std::endl;
    std::cout << "  Address:" << static_cast<void *>(curr) << std::endl;
    std::cout << "  Size:" << curr->size << std::endl;
    std::cout << "  Status: " << (curr->allocated ? "Allocated" : "Free")
              << std::endl;
    std::printf("--------------------------------\n");
    ++index;
  }
}

void Heap::heapInit() {
  BlockHeader *first = reinterpret_cast<BlockHeader *>(heapBegin());
  first->size = heapSize() - sizeof(BlockHeader);
  first->allocated = false;
}

}; // namespace CustomHeap
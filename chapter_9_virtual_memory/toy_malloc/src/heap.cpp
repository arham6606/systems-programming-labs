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

std::byte *Heap::allocate(std::size_t current) {
  BlockHeader *block = findFreeBlock(current);
  if (block == nullptr) {
    return nullptr; // Out of memory
  }

  std::size_t remaining = block->size - current;

  // Split only if there's enough room left for a new header
  // plus at least one byte of payload for the free remainder.
  if (remaining > sizeof(BlockHeader)) {
    // New free block starts right after the allocated portion
    BlockHeader *newFree = reinterpret_cast<BlockHeader *>(
        pointerOfPayloadSectionToUser(block) + current);
    newFree->size = remaining - sizeof(BlockHeader);
    newFree->allocated = false;

    // Shrink the current block to exactly what was requested
    block->size = current;
  }
  // else: remaining is too small to split; give the user the whole block
  //       (internal fragmentation — can't be helped here)

  block->allocated = true;
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

void Heap::deallocate(std::byte *ptr) {
  if (ptr == nullptr)
    return;

  BlockHeader *header = pointerToHeaderSection(ptr);
  header->allocated = false;
  coalescingFreeBlocks();
}

void Heap::coalescingFreeBlocks() {
  BlockHeader *curr = reinterpret_cast<BlockHeader *>(heapBegin());
  while (curr != nullptr) {
    BlockHeader *nxt = nextBlock(curr);
    if (nxt != nullptr && !curr->allocated && !nxt->allocated) {
      // Merge nxt into curr
      curr->size += sizeof(BlockHeader) + nxt->size;
      // Stay at curr; the new next might also be free
    } else {
      curr = nxt;
    }
  }
}

}; // namespace CustomHeap
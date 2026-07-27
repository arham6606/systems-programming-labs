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

}; // namespace CustomHeap
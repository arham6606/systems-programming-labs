#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>

namespace {

alignas(std::max_align_t) std::byte heap[1024];

std::byte *heapBegin() { return heap; }

std::byte *heapEnd() { return heap + sizeof(heap); }

std::size_t heapSize() { return sizeof(heap); }

}; // namespace

namespace HeapPerBlock {
struct BlockHeader {
  size_t size;
  bool allocated;
};

std::byte *pointerOfPayloadSectionToUser(BlockHeader *header) {
  return reinterpret_cast<std::byte *>(header) + sizeof(BlockHeader);
}

BlockHeader *pointerToHeaderSection(std::byte *user) {
  return reinterpret_cast<BlockHeader *>(user - sizeof(BlockHeader));
}

} // namespace HeapPerBlock

int main() {

  // --- sizeof ---
  std::printf("sizeof(BlockHeader) = %zu bytes\n",
              sizeof(HeapPerBlock::BlockHeader));

  // --- alignment ---
  std::printf("alignof(BlockHeader) = %zu bytes\n",
              alignof(HeapPerBlock::BlockHeader));

  // --- Place one header at the start of the heap ---
  HeapPerBlock::BlockHeader *first =
      reinterpret_cast<HeapPerBlock::BlockHeader *>(heapBegin());
  first->size = heapSize() - sizeof(HeapPerBlock::BlockHeader);
  first->allocated = false;

  std::cout << "Heap Size:" << heapSize() << std::endl;
  std::cout << "First Size:" << first->size << std::endl;

  // --- Where does user memory start? ---
  std::byte *userPtr = HeapPerBlock::pointerOfPayloadSectionToUser(first);

  std::printf("\n--- Layout ---\n");
  std::printf("Heap start:       %p\n", static_cast<void *>(heapBegin()));
  std::printf("Header address:   %p\n", static_cast<void *>(first));
  std::printf("User bytes start: %p\n", static_cast<void *>(userPtr));
  std::printf("Heap end:         %p\n", static_cast<void *>(heapEnd()));

  // --- Offsets ---
  std::ptrdiff_t headerOffset =
      reinterpret_cast<std::byte *>(first) - heapBegin();
  std::ptrdiff_t userOffset = userPtr - heapBegin();

  std::printf("\n--- Offsets from heap start ---\n");
  std::printf("Header offset:  %td bytes\n", headerOffset);
  std::printf("User offset:    %td bytes\n", userOffset);
  std::printf("Header size:    %zu bytes\n", sizeof(HeapPerBlock::BlockHeader));

  // --- Round-trip test ---
  std::printf("\n--- Round-trip test ---\n");
  HeapPerBlock::BlockHeader *recovered =
      HeapPerBlock::pointerToHeaderSection(userPtr);
  std::printf("Original header:  %p\n", static_cast<void *>(first));
  std::printf("Recovered header: %p\n", static_cast<void *>(recovered));
  std::printf("Match: %s\n", (recovered == first) ? "YES" : "NO");

  // --- Visual diagram ---
  std::printf("\n--- Memory Layout ---\n");
  std::printf("+--------------------+  <-- heapBegin()  = %p\n",
              static_cast<void *>(heapBegin()));
  std::printf("| BlockHeader        |      size=%zu, allocated=%s\n",
              first->size, first->allocated ? "true" : "false");
  std::printf("+--------------------+  <-- userPtr     = %p\n",
              static_cast<void *>(userPtr));
  std::printf("| User bytes         |      %zu bytes available\n", first->size);
  std::printf("| (payload)          |\n");
  std::printf("+--------------------+  <-- heapEnd()   = %p\n",
              static_cast<void *>(heapEnd()));

  return 0;
}
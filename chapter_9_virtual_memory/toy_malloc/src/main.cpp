#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>

namespace {

alignas(std::max_align_t) std::byte heap[1024 * 1024];

std::byte *heapBegin() { return heap; }

std::byte *heapEnd() { return heap + sizeof(heap); }

std::size_t heapSize() { return sizeof(heap); }

}; // namespace

int main() {

  std::cout << "Heap begins at: " << static_cast<void *>(heapBegin())
            << std::endl;
  std::cout << "Heap ends at: " << static_cast<void *>(heapEnd()) << std::endl;
  std::cout << "Heap Size: " << heapSize() << std::endl;
}
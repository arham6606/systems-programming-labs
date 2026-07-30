#include "heap.hpp"

int main() {
  CustomHeap::Heap obj;
  obj.heapInit();

  std::printf("=== Before allocation ===\n");
  obj.printHeap();

  std::byte *p1 = obj.allocate(64);
  std::byte *p2 = obj.allocate(128);
  std::byte *p3 = obj.allocate(256);

  std::printf("\nallocate(64)  -> %p\n", static_cast<void *>(p1));
  std::printf("allocate(128) -> %p\n", static_cast<void *>(p2));
  std::printf("allocate(256) -> %p\n\n", static_cast<void *>(p3));

  std::printf("=== After allocations ===\n");
  obj.printHeap();
}
#include "heap.hpp"

int main() {
  CustomHeap::Heap obj;
  std::cout << "Inializing Heap" << std::endl;
  obj.heapInit();

  std::cout << "Before Allocation" << std::endl;
  obj.printHeap();

  std::byte *allocation_1 = obj.allocate(64);
  std::cout << "Allocate(64) -> " << static_cast<void *>(allocation_1)
            << std::endl;

  std::cout << "After Allocation" << std::endl;
  obj.printHeap();

  std::cout << "Second Allocation" << std::endl;
  std::byte *allocation_2 = obj.allocate(32);
  if (allocation_2 == nullptr) {
    std::cout << "Allocation 2 failed" << std::endl;
  }
}
#include "heap.hpp"

int main() {
  CustomHeap::Heap obj;
  std::cout << "Heap size:" << obj.heapSize() << std::endl;
  std::cout << "Heap begin:" << obj.heapBegin() << std::endl;
  std::cout << "Heap end:" << obj.heapEnd() << std::endl;

  CustomHeap::BlockHeader *first =
      reinterpret_cast<CustomHeap::BlockHeader *>(obj.heapBegin());
  first->size = obj.heapSize() - sizeof(CustomHeap::BlockHeader);
  first->allocated = false;

  if (!first->allocated) {
    std::cout << "Block is allocated" << std::endl;
  }

  

  int index = 0;

  while (first != nullptr) {
    std::cout << "First size:" << first->size << std::endl;
    first = obj.nextBlock(first);
    index++;
  }
  std::cout << "Index:" << index << std::endl;
}
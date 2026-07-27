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

  std::cout << "First size:" << first->size << std::endl;

  if (!first->allocated) {
    std::cout << "Block is allocated" << std::endl;
  }
  
}
#include "test.hpp"

CustomHeap::Heap obj;
// Test 1: Allocate until out of memory
void testExhaustion() {
  obj.heapInit();
  std::printf("=== TEST 1: Exhaustion ===\n");
  int count = 0;
  while (obj.allocate(64) != nullptr) {
    ++count;
  }
  std::printf("Allocated %d blocks of 64 bytes before Out Of Memory\n", count);
  obj.printHeapSummary();
}

// Test 2: Alternate allocate / free to visualize fragmentation
void testAlternate() {
  obj.heapInit();
  std::printf("\n=== TEST 2: Alternate A/F ===\n");

  std::byte *a = obj.allocate(64);
  std::printf("After allocate(64):\n");
  obj.printHeap();

  obj.deallocate(a);
  std::printf("After deallocate (should be one free block):\n");
  obj.printHeap();

  a = obj.allocate(64);
  std::printf("After second allocate(64):\n");
  obj.printHeap();

  obj.deallocate(a);
  std::printf("After second deallocate:\n");
  obj.printHeap();
}

// Test 3: Free everything -> one block
void testFreeAll() {
  obj.heapInit();
  std::printf("\n=== TEST 3: Free Everything ===\n");

  std::byte *p1 = obj.allocate(100);
  std::byte *p2 = obj.allocate(200);
  std::byte *p3 = obj.allocate(300);
  std::printf("After 3 allocations:\n");
  obj.printHeap();

  obj.deallocate(p1);
  obj.deallocate(p2);
  obj.deallocate(p3);
  std::printf("After freeing all (should be ONE block):\n");
  obj.printHeap();
}

// Test 4: Random pattern
void testRandom() {
  obj.heapInit();
  std::printf("\n=== TEST 4: Random Pattern ===\n");

  std::byte *p30 = obj.allocate(30);
  std::byte *p120 = obj.allocate(120);
  std::byte *p90 = obj.allocate(90);
  std::byte *p512 = obj.allocate(512);
  std::byte *p80 = obj.allocate(80);
  std::byte *p64 = obj.allocate(64);
  std::printf("After random allocations:\n");
  obj.printHeap();

  std::printf("\nFreeing 120, 80, 30...\n");
  obj.deallocate(p120);
  obj.deallocate(p80);
  obj.deallocate(p30);
  std::printf("After selective frees (fragmentation + coalescing):\n");
  obj.printHeap();

  std::printf("\nAllocate 200 (should fit in coalesced gap)...\n");
  std::byte *p200 = obj.allocate(200);
  std::printf("allocate(200) -> %p\n", static_cast<void *>(p200));
  obj.printHeap();
}

// Test 5: Edge cases
void testEdgeCases() {
  obj.heapInit();
  std::printf("\n=== TEST 5: Edge Cases ===\n");

  // allocate(0)
  std::byte *z = obj.allocate(0);
  std::printf("allocate(0)      -> %p  (expected: nullptr)\n",
              static_cast<void *>(z));

  // allocate(entire heap)
  std::byte *huge = obj.allocate(obj.heapSize());
  std::printf(
      "allocate(heap)   -> %p  (expected: nullptr, header needs space)\n",
      static_cast<void *>(huge));

  // allocate(massive)
  std::byte *massive = obj.allocate(1024ull * 1024 * 100);
  std::printf("allocate(100MB)  -> %p  (expected: nullptr)\n",
              static_cast<void *>(massive));

  // free(nullptr)
  std::printf("deallocate(nullptr)... ");
  obj.deallocate(nullptr);
  std::printf("OK (no crash)\n");

  // double free
  std::byte *p = obj.allocate(64);
  obj.deallocate(p);
  std::printf("Double-free test:\n");
  obj.deallocate(p); // should print guard message
}
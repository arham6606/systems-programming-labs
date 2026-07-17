#include "MMU.hpp"
#include <bitset>
#include <iostream>

int main() {

  // testing started

  /*
  // Virtual address setup
  uint16_t virtual_address = 0x4498;
  uint8_t virtual_page_number = virtual_address >> 8;
  uint8_t page_offset = virtual_address & 0x00FF;

  std::cout << "VA: " << std::bitset<16>(virtual_address) << " :"
  << int(virtual_address) << std::endl;
  std::cout << "VPN: " << std::bitset<8>(virtual_page_number) << " :"
  << int(virtual_page_number) << std::endl;
  std::cout << "Offset: " << std::bitset<8>(page_offset) << " :"
  << int(page_offset) << std::endl;

  // Physical memory setup
  size_t num_frames = 8;
  size_t page_size = 256;

  PhysicalMemory physical_memory(num_frames, page_size);
  std::cout << "Number of frames: " << physical_memory.getNumberOfFrames()
  << std::endl;
  std::cout << "Page size: " << physical_memory.getPageSize() << std::endl;

  // Write test data
  physical_memory.write(5, 100, 42);
  physical_memory.write(5, 101, 65);

  uint8_t value_1 = physical_memory.read(5, 100);
  uint8_t value_2 = physical_memory.read(5, 101);
  uint8_t value_3 = physical_memory.read(5, 102);

  std::cout << "Value at frame 5, offset 100: " << static_cast<int>(value_1)
  << std::endl;
  std::cout << "Value at frame 5, offset 101: " << static_cast<int>(value_2)
  << std::endl;
  std::cout << "Value at frame 5, offset 102: " << static_cast<int>(value_3)
  << " (zeroed out)" << std::endl;

  // Page table setup
  PageTable pt(256);
  pt.map(0, 5);
  pt.map(1, 10);
  pt.map(3, 2);
  pt.map(int(virtual_page_number), 7);

  // Page table lookups
  auto f0 = pt.lookUp(0);
  auto f1 = pt.lookUp(1);
  auto f2 = pt.lookUp(2);
  auto f3 = pt.lookUp(3);
  auto f68 = pt.lookUp(int(virtual_page_number));

  std::cout << "VPN 0 → Frame " << (int)f0.value() << "\n";
  std::cout << "VPN 1 → Frame " << (int)f1.value() << "\n";
  std::cout << "VPN 2 → " << (f2 ? "valid" : "INVALID") << "\n";
  std::cout << "VPN 3 → Frame " << (int)f3.value() << "\n";
  std::cout << "VPN 68 → Frame " << (int)f68.value() << "\n";

  // Address translation
  uint16_t physical_address =
  (static_cast<uint16_t>(f68.value()) << 8 | page_offset);

  std::cout << "Physical Address: 0x" << std::hex << physical_address
  << std::dec << std::endl;
  */

  // testing ended

  // real logic

  // Create MMU: 256 virtual pages, 64 physical frames, 256 bytes per page
  MMU mmu(256, 64, 256);

  // OS sets up page table mappings (normally the OS would do this)
  mmu.getPageTable().map(0x12, 5);  // VPN 0x12 → Frame 5
  mmu.getPageTable().map(0x00, 10); // VPN 0x00 → Frame 10
  mmu.getPageTable().map(0x01, 2);  // VPN 0x01 → Frame 2

  // === CPU asks MMU to write at virtual address 0x1234 ===
  uint16_t vaddr = 0x1234;
  uint8_t dataToWrite = 0xAB;

  std::cout << "CPU: Write 0x" << std::hex << (int)dataToWrite << " to VA 0x"
            << vaddr << "\n";

  if (mmu.write(vaddr, dataToWrite)) {
    std::cout << "MMU: Write succeeded\n";
  } else {
    std::cout << "MMU: PAGE FAULT!\n";
  }

  // === CPU asks MMU to read back ===
  uint8_t dataRead = 0;
  if (mmu.read(vaddr, dataRead)) {
    std::cout << "CPU: Read back 0x" << (int)dataRead << " from VA 0x" << vaddr
              << "\n";
  } else {
    std::cout << "MMU: PAGE FAULT on read!\n";
  }
  // === Test unmapped address (page fault) ===
  uint16_t badAddr = 0x0200; // VPN 0x02, not mapped
  uint8_t dummy;

  if (!mmu.read(badAddr, dummy)) {
    std::cout << "\nCPU: Read VA 0x" << badAddr << "\n";
    std::cout << "MMU: PAGE FAULT — VPN 0x02 not mapped\n";
  }

  return 0;
}
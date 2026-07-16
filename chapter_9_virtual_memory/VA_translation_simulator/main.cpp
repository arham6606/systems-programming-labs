#include "page_table.hpp"
#include "physical_memory.hpp"
#include <bitset>
#include <iostream>

int main() {
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
  pt.map(68, 7);

  // Page table lookups
  auto f0 = pt.lookUp(0);
  auto f1 = pt.lookUp(1);
  auto f2 = pt.lookUp(2);
  auto f3 = pt.lookUp(3);
  auto f68 = pt.lookUp(68);

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

  return 0;
}
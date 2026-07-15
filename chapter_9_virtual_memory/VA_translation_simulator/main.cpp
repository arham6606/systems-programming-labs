#include "page_table.hpp"
#include "physical_memory.hpp"
#include <iostream>

int main() {

  size_t num_frames = 8;
  size_t page_size = 256;

  PhysicalMemory physical_memory(num_frames,
                                 page_size); // 8 frames, 256 bytes per frame
  std::cout << "Number of frames: " << physical_memory.getNumberOfFrames()
            << std::endl;
  std::cout << "Page size: " << physical_memory.getPageSize() << std::endl;

  physical_memory.write(5, 100, 42);
  physical_memory.write(5, 101, 65);

  uint8_t value_1 = physical_memory.read(5, 100);
  uint8_t value_2 = physical_memory.read(5, 101);

  // by default uint8_t is gives char type if we dont do casting
  std::cout << value_1 << std::endl;
  std::cout << value_2 << std::endl;

  std::cout << "Value at frame 5, offset 100: " << static_cast<int>(value_1)
            << std::endl;
  std::cout << "Value at frame 5, offset 101: " << static_cast<char>(value_2)
            << std::endl;

  // it will give 0 because we have not write anything to byte of that frame
  // because we have zeroed out each byte of each frame
  uint8_t value_3 = physical_memory.read(5, 102);
  std::cout << "Value at frame 5, offset 102: " << static_cast<int>(value_3)
            << std::endl;

  // this will give error
  //  physical_memory.write(66, 456, 33);
  
  PageTable pt(256); // 256 virtual pages

  // Hardcode the mappings
  pt.map(0, 5);
  pt.map(1, 10);
  pt.map(3, 2);
  // VPN 2 stays invalid (default)

  // Test lookups
  auto f0 = pt.lookUp(0);
  auto f1 = pt.lookUp(1);
  auto f2 = pt.lookUp(2); // Invalid!
  auto f3 = pt.lookUp(3);

  std::cout << "VPN 0 → Frame " << (int)f0.value() << "\n";      // 5
  std::cout << "VPN 1 → Frame " << (int)f1.value() << "\n";      // 10
  std::cout << "VPN 2 → " << (f2 ? "valid" : "INVALID") << "\n"; // INVALID
  std::cout << "VPN 3 → Frame " << (int)f3.value() << "\n";      // 2

  return 0;
}
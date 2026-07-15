#include "physical_memory.hpp"
#include <iostream>

int main() {
  PhysicalMemory physicalMemory(8, 256); // 8 frames, 256 bytes per frame
  std::cout << "Number of frames: " << physicalMemory.getNumberOfFrames()
            << std::endl;
  std::cout << "Page size: " << physicalMemory.getPageSize() << std::endl;

  physicalMemory.write(5, 100, 42);
  physicalMemory.write(5, 101, 65);

  uint8_t value1 = physicalMemory.read(5, 100);
  uint8_t value2 = physicalMemory.read(5, 101);

  // by default uint8_t is gives characters if we dont do casting
  std::cout << value1 << std::endl;
  std::cout << value2 << std::endl;

  std::cout << "Value at frame 5, offset 100: " << static_cast<int>(value1)
            << std::endl;
  std::cout << "Value at frame 5, offset 101: " << static_cast<char>(value2)
            << std::endl;

  // it will give 0 because we have not write anything to byte of that frame
  // because we have zeroed out each byte of each frame
  uint8_t value3 = physicalMemory.read(5, 102);
  std::cout << "Value at frame 5, offset 102: " << static_cast<int>(value3)
            << std::endl;

  // this will give error
  //  physicalMemory.write(66, 456, 33);

  return 0;
}
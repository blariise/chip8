#ifndef CHIP8_H
#define CHIP8_H

#include <string_view>
#include <random>
#include <chrono>
#include <fstream>
#include <array>
#include <stdint.h>

typedef struct Chip8 {
  std::array<uint8_t, 4096> memory;  // memory of 4kB
  std::array<uint8_t, 16> V;     // cpu register from V0 - VF
  std::uint8_t SP; // level of the stack (stack pointer)
  std::array<uint16_t, 16> stack;

  std::uint16_t PC; // pointer to current instruction in memory (program counter)
  std::uint16_t I; // pointer to location in memory (index register)
  std::uint16_t opcode;


  std::array<uint8_t, 64 * 32> display; // display 64x32 px
  std::uint8_t delay_timer;
  std::uint8_t sound_timer;

  /* 
    std::default_random_engine seed;
    std::uniform_int_distribution<unsigned char> random_byte;
  */

  void init(); // Initializing and reseting machine
  void cycle(); // Fetch, Decode, Execute (One cycle)
  //void loadRom(std::string_view filename);

} Chip8;
#endif // CHIP_8_H


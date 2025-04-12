#ifndef CHIP8_H
#define CHIP8_H

#include <string_view>
#include <random>
#include <fstream>
#include <array>
#include <cstdint>

class Chip8 {
  public:
    Chip8();
    void cycle(); // one cycle of chip8
    const std::array<bool, 64 * 32>& getDisplay() const;
    void setKeyState(std::size_t key, bool pressed);
    void loadRom(std::string_view filename);

  private:
    std::array<std::uint8_t, 4096> memory {};  // memory of 4kB
    std::array<std::uint8_t, 16> V {};     // cpu register from V0 - VF
    std::array<std::uint16_t, 16> stack {};
    std::uint8_t SP {}; // level of the stack (stack pointer)

    std::uint16_t PC { 0x200u }; // pointer to current instruction in memory (program counter), at 0x200(512) program starts
    std::uint16_t I {}; // pointer to location in memory (index register)
    std::uint16_t opcode {};

    std::array<bool, 64 * 32> display {}; // display 64x32 px
    std::uint8_t delay_timer {};
    std::uint8_t sound_timer {};
    std::array<bool, 16> keyboard {}; // track which key is pressed

    void loadFont(); // Load font into memory, first 80 addresses

    std::mt19937 mt { std::random_device{}() };
    std::uniform_int_distribution<std::uint8_t> random_byte { 0, 255 };
};
#endif // CHIP8_H


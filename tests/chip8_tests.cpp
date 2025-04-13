#include <gtest/gtest.h>
#include <cstdint>

#include "../chip8.h"

class Chip8test : public testing::Test {
  protected:
    Chip8 chip8 {};
    
    void loadOpcode(std::uint16_t m_opcode) {
      chip8.memory[chip8.PC] = (m_opcode >> 8) & 0xFF;
      chip8.memory[chip8.PC + 1] = m_opcode & 0xFF;
      chip8.cycle();
    }
};

TEST_F(Chip8test, CLS) {
  for (std::size_t i { 0 }; i < 64 * 32; ++i)
    chip8.display[i] = 1;
  
  loadOpcode(0x00E0);
  for (const bool pixel : chip8.getDisplay()) {
    EXPECT_FALSE(pixel);
  }
}

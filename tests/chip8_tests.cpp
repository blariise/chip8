#include <gtest/gtest.h>
#include <cstdint>

#include "chip8.h"

class Chip8test : public testing::Test {
  protected:
    Chip8 chip;
    
    void loadOpcode(std::uint16_t opcode) {
      chip.memory[chip.PC] = (opcode >> 8) & 0xFF;
      chip.memory[chip.PC + 1] = opcode & 0xFF;
      chip.cycle();
    }
};

// 00E0
TEST_F(Chip8test, CLS) {
  for (std::size_t i { 0 }; i < 64 * 32; ++i)
    chip.display[i] = 1;
  
  loadOpcode(0x00E0);
  for (const bool pixel : chip.getDisplay()) {
    EXPECT_FALSE(pixel);
  }
}

// 1nnn
TEST_F(Chip8test, JP) {
  loadOpcode(0x133F);
  EXPECT_EQ(chip.PC, 0x33F);
}

// 2nnn
TEST_F(Chip8test, CALL) {
  std::uint16_t pc_before_instruction { chip.PC } ; 
  loadOpcode(0x2432);
  
  EXPECT_EQ(chip.PC, 0x432);
  EXPECT_EQ(chip.SP, 1);
  EXPECT_EQ(chip.stack[chip.SP], pc_before_instruction + 2);
}

TEST_F(Chip8test, CALL_nested) {
  loadOpcode(0x2100);
  EXPECT_EQ(chip.PC, 0x100);
  EXPECT_EQ(chip.SP, 1);
  EXPECT_EQ(chip.stack[chip.SP], 0x202); // 0x200 starts chip8
  
  loadOpcode(0x2200);
  EXPECT_EQ(chip.PC, 0x200);
  EXPECT_EQ(chip.SP, 2);
  EXPECT_EQ(chip.stack[chip.SP], 0x102);
}

TEST_F(Chip8test, SE_Vx_Byte) {
  chip.PC = 0x100;
  chip.V[3] = 0x52;
  loadOpcode(0x3352);
  EXPECT_EQ(chip.PC, 0x104);

  // V[x] != 0x54, so PC increments only by 2
  loadOpcode(0x354);
  EXPECT_EQ(chip.PC, 0x106);
}

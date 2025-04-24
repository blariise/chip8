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

// 3xkk
TEST_F(Chip8test, SE_Vx_Byte) {
  chip.PC = 0x100;
  chip.V[3] = 0x52;
  loadOpcode(0x3352);
  EXPECT_EQ(chip.PC, 0x104);

  // V[x] != 0x54, so PC increments only by 2
  loadOpcode(0x354);
  EXPECT_EQ(chip.PC, 0x106);
}

// 4xkk
TEST_F(Chip8test, SNE_Vx_Byte) {
  chip.PC = 0x100;
  chip.V[8] = 0x43;
  loadOpcode(0x483A);
  EXPECT_EQ(chip.PC, 0x104);

  loadOpcode(0x4843);
  EXPECT_EQ(chip.PC, 0x106);
}

// 5xy0
TEST_F(Chip8test, SE_Vx_Vy) {
  chip.PC = 0x100;
  chip.V[6] = 0x4D;
  chip.V[4] = 0x4D;
  loadOpcode(0x5640);
  EXPECT_EQ(chip.PC, 0x104);

  chip.V[7] = 0x24;
  loadOpcode(0x5670);
  EXPECT_EQ(chip.PC, 0x106);
}

// 6xkk
TEST_F(Chip8test, LD_Vx_Byte) {
  chip.V[2] = 0xBD;
  loadOpcode(0x62AD);
  EXPECT_EQ(chip.V[2], 0xAD);
}

// 7xkk
TEST_F(Chip8test, ADD_Vx_Byte) {
  chip.V[0xA] = 0x53;
  loadOpcode(0x7A4A);
  // 0x53 + 0x4A = 0x9D
  EXPECT_EQ(chip.V[0xA], 0x9D);

  loadOpcode(0x7AEE);
  // 0x9D + 0xEE = 18B, but with overflow 18B = 395, 395 % 256 = 139 = 8B
  EXPECT_EQ(chip.V[0xA], 0x8B);
}

// 8xy0
TEST_F(Chip8test, LD_Vx_Vy) {
  chip.V[0xD] = 0x6D;
  chip.V[0x1] = 0x23;
  loadOpcode(0x8D10);
  EXPECT_EQ(chip.V[0xD], 0x23);
}

// 8xy1
TEST_F(Chip8test, OR_Vx_Vy) {
  chip.V[0xD] = 0x5A;
  chip.V[0x1] = 0x3C;
  loadOpcode(0x8D11);
  EXPECT_EQ(chip.V[0xD], 0x7E);

  chip.V[0xD] = 0x0F;
  chip.V[0x1] = 0xF0;
  loadOpcode(0x8D11);
  EXPECT_EQ(chip.V[0xD], 0xFF);
}

// 8xy2
TEST_F(Chip8test, AND_Vx_Vy) {
  chip.V[0xD] = 0x5A;
  chip.V[0x1] = 0x3C;
  loadOpcode(0x8D12);
  EXPECT_EQ(chip.V[0xD], 0x18);

  chip.V[0xD] = 0xF0;
  chip.V[0x1] = 0x0F;
  loadOpcode(0x8D12);
  EXPECT_EQ(chip.V[0xD], 0x00);
}

// 8xy3
TEST_F(Chip8test, XOR_Vx_Vy) {
  chip.V[0xD] = 0x5A;
  chip.V[0x1] = 0x3C;
  loadOpcode(0x8D13);
  EXPECT_EQ(chip.V[0xD], 0x66);

  chip.V[0xD] = 0xFF;
  chip.V[0x1] = 0x0F;
  loadOpcode(0x8D13);
  EXPECT_EQ(chip.V[0xD], 0xF0);
}

// 8xy4
TEST_F(Chip8test, ADD_Vx_Vy) {
  chip.V[0xD] = 0xC7;
  chip.V[0x1] = 0x9E;
  loadOpcode(0x8D14);
  EXPECT_EQ(chip.V[0xD], 0x65);
  EXPECT_EQ(chip.V[0xF], 0x1);

  chip.V[0xD] = 0xAB;
  chip.V[0x1] = 0xF2;
  loadOpcode(0x8D14);
  EXPECT_EQ(chip.V[0xD], 0x9D);
  EXPECT_EQ(chip.V[0xF], 0x1);

  chip.V[0xD] = 0x3A;
  chip.V[0x1] = 0x47;
  loadOpcode(0x8D14);
  EXPECT_EQ(chip.V[0xD], 0x81);
  EXPECT_EQ(chip.V[0xF], 0x0);
}

// 8xy5
TEST_F(Chip8test, SUB_Vx_Vy) {
  chip.V[0xD] = 0x50;
  chip.V[0x1] = 0x20;
  loadOpcode(0x8D15);
  EXPECT_EQ(chip.V[0xD], 0x30);
  EXPECT_EQ(chip.V[0xF], 0x1);

  chip.V[0xD] = 0x10;
  chip.V[0x1] = 0x30;
  loadOpcode(0x8D15);
  EXPECT_EQ(chip.V[0xD], 0xE0);
  EXPECT_EQ(chip.V[0xF], 0x0);

  chip.V[0xD] = 0x77;
  chip.V[0x1] = 0x77;
  loadOpcode(0x8D15);
  EXPECT_EQ(chip.V[0xD], 0x00);
  EXPECT_EQ(chip.V[0xF], 0x0);
}

// 8xy6
TEST_F(Chip8test, SHR_Vx) {
  chip.V[0xD] = 0xB5;
  loadOpcode(0x8D16);
  EXPECT_EQ(chip.V[0xD], 0x5A);
  EXPECT_EQ(chip.V[0xF], 0x1);

  chip.V[0xD] = 0x4;
  loadOpcode(0x8D16);
  EXPECT_EQ(chip.V[0xD], 0x2);
  EXPECT_EQ(chip.V[0xF], 0x0);
}

// 8xy7
TEST_F(Chip8test, SUBN_Vx_Vy) {
  chip.V[0xD] = 0x30;
  chip.V[0x1] = 0x80;
  loadOpcode(0x8D17);
  EXPECT_EQ(chip.V[0xD], 0x50);
  EXPECT_EQ(chip.V[0xF], 0x1);

  chip.V[0xD] = 0xF0;
  chip.V[0x1] = 0x40;
  loadOpcode(0x8D17);
  EXPECT_EQ(chip.V[0xD], 0x50);
  EXPECT_EQ(chip.V[0xF], 0x0);
}

// 8xyE
TEST_F(Chip8test, SHL_Vx) {
  chip.V[0xD] = 0x8B;
  loadOpcode(0x8D1E);
  EXPECT_EQ(chip.V[0xD], 0x16);
  EXPECT_EQ(chip.V[0xF], 0x1);

  chip.V[0xD] = 0x7B;
  loadOpcode(0x8D1E);
  EXPECT_EQ(chip.V[0xD], 0xF6);
  EXPECT_EQ(chip.V[0xF], 0x0);
}

// 9xy0
TEST_F(Chip8test, SNE_Vx_Vy) {
  chip.PC = 0x100;
  chip.V[0xD] = 0x6A;
  chip.V[0xA] = 0x76;
  loadOpcode(0x9DA0);
  EXPECT_EQ(chip.PC, 0x104);

  chip.V[0xA] = 0x6A;
  loadOpcode(0x9DA0);
  EXPECT_EQ(chip.PC, 0x106);
}

// Annn
TEST_F(Chip8test, LD_I) {
  loadOpcode(0xA932);
  EXPECT_EQ(chip.I, 0x932);

  loadOpcode(0xA000);
  EXPECT_EQ(chip.I, 0x000);

  loadOpcode(0xAFFF);
  EXPECT_EQ(chip.I, 0xFFF);
}

// Bnnn
TEST_F(Chip8test, JP_V0) {
  chip.V[0x0] = 0xF1;
  loadOpcode(0xBAA6);
  EXPECT_EQ(chip.PC, 0xB97);

  chip.V[0x0] = 0xFF;
  loadOpcode(0xBFFF);
  EXPECT_EQ(chip.PC, 0x10FE);
}


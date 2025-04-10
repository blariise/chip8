#include "chip8.h"

std::array<std::uint8_t, 80> fontSet {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

// Initializing and reseting machine
void Chip8::init() {

  PC = 0x200; // 0x200 (512) Start Chip-8
  opcode = 0;
  I = 0;
  SP = 0;

  // Clear the display
  for (int i { 0 }; i < 2048; ++i)
    display[i] = 0;

  // Clear the stack and registers V0-VF
  for (int i { 0 }; i < 16; ++i) {
    stack[i] = 0;
    V[i] = 0;
  }
    
  // Clear memory
  for (int i { 0 }; i < 4096; ++i)
    memory[i] = 0;

  // Load fonts into memory
  for (int i { 0 }; i < 80; ++i)
    memory[i] = fontSet[i];

  delay_timer = 0;
  sound_timer = 0;
}

void Chip8::cycle() {
  
  // need to shift to left 8 bits, to make room for second instruction
  // example memory[PC]     = 0xFF after shifting its 0xFF00
  //         memory[PC + 1] = 0xA1
  //         so 0xFF00 | 0xFFA1
  // fetch opcode
  opcode = (memory[PC] << 8) | (memory[PC + 1]);

  PC += 2;  // increment by 2, because every instruction(opcode) is 2bytes

  std::uint16_t nnn = opcode & 0xFFFu; // A 12-bit value, the lowest 12 bits of the instruction
  std::uint8_t n    = opcode & 0xFu; // A 4-bit value, the lowest 4 bits of the instruction
  std::uint8_t x    = opcode & 0xF00u; // A 4-bit value, the lower 4 bits of the high byte of the instruction
  std::uint8_t y    = opcode & 0xF0u; // A 4-bit value, the upper 4 bits of the low byte of the instruction
  std::uint8_t kk   = opcode & 0xFFu; // An 8-bit value, the lowest 8 bits of the instruction

}

int main() {
  return 0;
}

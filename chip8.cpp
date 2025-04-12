#include "chip8.h"

std::array<std::uint8_t, 80> font_map {
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

std::array<std::uint8_t, 16> key_map {
  0x0, // 0
  0x1, // 1
  0x2, // 2
  0x3, // 3
  0x4, // 4
  0x5, // 5
  0x6, // 6
  0x7, // 7
  0x8, // 8
  0x9, // 9
  0xA, // A
  0xB, // B
  0xC, // C
  0xD, // D
  0xE, // E
  0xF,  // F
};

Chip8::Chip8() {}

// Initializing and reseting machine
void Chip8::loadFont() {
  // Load fonts into memory
  for (int i { 0 }; i < 80; ++i)
    memory[i] = font_map[i];
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
  std::uint8_t high_nibble = (opcode & 0xF000u) >> 12u;

  switch (high_nibble) {
    
    case 0x0:
      switch (kk) {
        
        case 0xE0:  // clear display
          for (int i { 0 }; i < 2048; ++i)
            display[i] = 0;
          break;

        case 0xEE:  // return from function
          PC = stack[SP];
          --SP;
          break;
        
        default:
          break;
      }

    case 0x1: // jump to 0nnn
      PC = nnn;
      break;

    case 0x2: // call function at 0nnn
      ++SP;
      stack[SP] = PC;
      PC = nnn;
      break;

    case 0x3: // skip next instruction if Vx == kk
      if (V[x] == kk)
        PC += 2;
      break;

    case 0x4: // skip next instruction if Vx != kk
      if (V[x] != kk)
        PC += 2;
      break;

    case 0x5: // skip next instruction if Vx == Vy
      if (V[x] == V[y])
        PC += 2;
      break;
    
    case 0x6:
      V[x] = kk;
      break;
    
    case 0x7:
      V[x] += kk;
      break;

    case 0x8:
      switch (n) {

        case 0x0:
          V[x] = V[y];
          break;

        case 0x1:
          V[x] = V[x] | V[y];
          break;

        case 0x2:
          V[x] = V[x] & V[y];
          break;

        case 0x3:
          V[x] = V[x] ^ V[y];
          break;

        case 0x4: {
          std::uint16_t sum = V[x] + V[y];
          V[x] = sum & 0xFFu;
          if (sum > 0xFFu)
            V[0xF] = 0xF;
          else
            V[0xF] = 0x0;
          break;
        }
        
        case 0x5:
          if (V[x] > V[y])
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[x] -= V[y];
          break;

        case 0x6:
          if (V[x] & 0x1 == 0x1)
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[x] /= 0x2;
          break;

        case 0x7:
          if (V[y] > V[x])
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[x] = V[y] - V[x];
          break;

        case 0xE:
          if (V[x] & 0x80 == 0x80)
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[x] *= 0x2;
          break;

        default:
          break;
      }

    case 0x9:
      if (V[x] != V[y])
        PC += 2;
      break;

    case 0xA:
      I = nnn;
      break;

    case 0xB:
      PC = nnn + V[0x0];
      break;
    
    case 0xC:
      V[x] = random_byte(mt) & kk;
      break;

    case 0xD: {
      V[0xF] = 0;
      std::uint8_t byte { 8 };
      // postion[ y * 64 + x ]
      // ...
      // postion[ (y + n-1) * 64 + x]
      for (std::uint8_t row { 0 }; row < n; ++row) {
        std::uint8_t sprite { memory[I + row] };
        std::uint8_t pixel_y = (y + row) % 32;  // height of display is 32 so if y exceedes it wraps around
        
        for (std::uint8_t col { 0 }; col < byte; ++col) {
          std::uint8_t pixel_x = (x + col) % 64; // width of display is 64 so if x exceedes it wraps around 
          bool pixel_sprite = (sprite >> (7 - col)) & 0x1u;
         
          std::uint8_t index { pixel_sprite && display[pixel_y * 64 + pixel_x] };
          if (pixel_sprite && display[index])
            V[0xFu] = 1;

          display[index] ^= pixel_sprite;
        }
      }
      break;
    }

    case 0xE:
      switch (kk) {

        case 0x9E:
          if (keyboard[V[x]])
            PC += 2;
          break;

        case 0xA1:
          if (!keyboard[V[x]])
            PC += 2;
          break;

        default:
          break;
      }

    case 0xF:
      switch (kk) {
      
        case 0x07:
          V[x] = delay_timer;
          break;

        case 0x0A: {
          bool exit = false;
          while(!exit) {
            for (std::uint8_t i { 0 }; i < 16; ++i) {
              if (keyboard[i]) {
                V[x] = key_map[i];
                exit = true;
              }
            }
          }
          break;
        }

        case 0x15:
          delay_timer = V[x];
          break;

        case 0x18:
          sound_timer = V[x];
          break;

        case 0x1E:
          I += V[x];
          break;

        case 0x29:
          I = V[x] * 0x05u;
          break;

        case 0x33: {
          std::uint8_t digit = V[x];
          
          memory[I + 2] = digit % 10;
          digit /= 10;
          memory[I + 1] = digit % 10;
          digit /= 10;
          memory[I] = digit % 10;
          
          break;
        }

        case 0x55:
          for (std::uint8_t i { 0 }; i < 16; ++i)
            memory[I + i] = V[i];
          break;

        case 0x65:
          for (std::uint8_t i { 0 }; i < 16; ++i)
            V[i] = memory[I + i];
          break;
        
        default:
          break;
      }

    default:
      break;
  }
}


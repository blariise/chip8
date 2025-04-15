#include <iostream>
#include "chip8.h"
#include <fstream>

std::array<std::uint8_t, 16> key_map {
  0x1,
  0x2,
  0x3,
  0xC,
  0x4,
  0x5,
  0x6,    // 1 2 3 C
  0xD,    // 4 5 6 D
  0x7,    // 7 8 9 E
  0x8,    // A 0 B F
  0x9,
  0xE,
  0xA,
  0x0,
  0xB,
  0xF,
};

Chip8::Chip8() {}

Chip8::~Chip8() {}

// copied from stackoveflow
int Chip8::loadRom(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open ROM");
        return -1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long rom_size = ftell(file);
    rewind(file);


    // Read ROM into memory starting at 0x200
    fread(&memory[0x200], sizeof(unsigned char), rom_size, file);
    fclose(file);

    return 0;
}

const std::array<bool, 64 * 32>& Chip8::getDisplay() const {
  return display;
}

void Chip8::setKeyState(std::size_t key, bool pressed) {
  if (key < std::size(keyboard))
    keyboard[key] = pressed;
}

void Chip8::cycle() {
  // need to shift to left 8 bits, to make room for second instruction
  // example memory[PC]     = 0xFF after shifting its 0xFF00
  //         memory[PC + 1] = 0xA1
  //         so 0xFF00 | 0xFFA1
  // fetch opcode
  opcode = (memory[PC] << 8) | (memory[PC + 1]);

  PC += 2;  // increment by 2, because every instruction(opcode) is 2bytes
  execute(opcode);
 // std::cout << "opcode: " << std::hex << "0x" << opcode << "\n";
 // for (int i {0}; i < std::size(V); ++i)
 //   std::cout << "V" << std::hex << i << std::dec << ": " << (int)V[i] << '\n';
}

void Chip8::updateTimers() {
  if (delay_timer > 0)
    --delay_timer;
  if (sound_timer > 0)
    --sound_timer;
}

void Chip8::execute(std::uint16_t opcode) {
  std::uint16_t nnn = opcode & 0xFFFu; // A 12-bit value, the lowest 12 bits of the instruction
  std::uint8_t n    = opcode & 0xFu; // A 4-bit value, the lowest 4 bits of the instruction
  std::uint8_t x    = (opcode & 0xF00u) >> 8u; // A 4-bit value, the lower 4 bits of the high byte of the instruction
  std::uint8_t y    = (opcode & 0xF0u) >> 4u; // A 4-bit value, the upper 4 bits of the low byte of the instruction
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
      break;

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
            V[0xF] = 1;
          else
            V[0xF] = 0;
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
          if ((V[x] & 0x1) == 0x1)
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
          if ((V[x] & 0x80) == 0x80)
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[x] *= 0x2;
          break;

        default:
          break;
      }
      break;

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
      uint8_t cord_x { static_cast<uint8_t>(V[x] % 64) };
      uint8_t cord_y { static_cast<uint8_t>(V[y] % 32) };
      
      for (std::uint8_t row { 0 }; row < n; ++row) {
        uint8_t sprite { memory[I + row] };

        for (std::uint8_t col { 0 }; col < 8; ++col) {
          if (sprite & (0x80 >> col)) {
            std::uint8_t pixel_x = (cord_x + col) % 64;
            std::uint8_t pixel_y = (cord_y + row) % 32;
            std::uint16_t pixel_index = pixel_y * 64 + pixel_x;

            if (display[pixel_index])
              V[0xF] = 1;
            display[pixel_index] ^= 1;
          }
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
      break;

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
          for (int i { 0 }; i <= x; ++i)
            memory[I + i] = V[i];
          break;

        case 0x65:
          for (int i { 0 }; i <= x; ++i)
            V[i] = memory[I + i];
          break;
        
        default:
          break;
      }
      break;

    default:
      break;
  }
}


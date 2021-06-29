#include <random>
#include <chrono>
#include <fstream>

#include "Chip8.h"

unsigned char fontSet[80]{
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

    PC = 0x200; // 0x200 (512) Start of most Chip-8 programs
    opcode = 0;
    I = 0;
    SP = 0;

    // Clear the display
    for (int i{0}; i < 2048; ++i) {   
           display[i] = 0; 
    }

    // Clear the stack and registers V0-VF
    for (int i{0}; i < 16; ++i) {
        stack[i] = 0;
        V[i] = 0;
    }
    
    // Clear memory
    for (int i{0}; i < 4096; ++i) {
        memory[i] = 0;
    }

    // Load fonts into memory
    for (int i{0}; i < 80; ++i) {
        memory[i] = fontSet[i];
    }

    delayTimer = 0;
    soundTimer = 0;

    // Init RNG
    std::default_random_engine seed(std::chrono::system_clock::now().time_since_epoch().count());
    randomByte = std::uniform_int_distribution<unsigned char>(1,255U);
}

void Chip8::loadRom(char const *filename) {

    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        std::streampos size = file.tellg();
        char *buffer = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (int i{0}; i < size; ++i) {
            memory[0x200 + i] = buffer[i];
        }
        delete[] buffer;
    }
}

void Chip8::cycle() {

    //Fetch opcode
    opcode = memory[PC] << 8 | memory[PC + 1]; 

    PC += 2;

    unsigned char nnn = opcode & 0x0FFFU; // A 12-bit value, the lowest 12 bits of the instruction
    unsigned char n = opcode & 0x000F; // A 4-bit value, the lowest 4 bits of the instruction
    unsigned char x = (opcode & 0x0F00U) >> 8U; // A 4-bit value, the lower 4 bits of the high byte of the instruction
    unsigned char y = (opcode & 0x00F0U) >> 4U; // A 4-bit value, the upper 4 bits of the low byte of the instruction
    unsigned char kk = opcode & 0x00FFU; // An 8-bit value, the lowest 8 bits of the instruction
    

    switch (opcode & 0xF000) {
        case 0: {
            if (opcode == 0x00E0) {
                for (int i{0}; i < 2048; ++i) {
                    display[i] = 0;
                }
            } else if (opcode == 0x00EE) {
                --SP;
                PC = stack[SP];
            }
            break;
        }
        case 0x1000: {
            PC = nnn; 
            break;
        }
        case 0x2000: {
            stack[SP] = PC;
            ++SP;
            PC = nnn;
            break;
        }
        case 0x3000: {
            if (V[x] == kk) {
                PC += 2;
            }
            break;
        }
        case 0x4000: {
            if (V[x] != kk) {
                PC += 2;
            }
            break;
        }
        case 0x5000: {
            if (V[x] == V[y]) {
                PC += 2;
            }
            break;
        }
        case 0x6000: {
            V[x] = kk;
            break;
        }
        case 0x7000: {
            V[x] += kk;
            break;
        }
        case 0x8000: {
            switch (n) {
                case 0x0000: {
                    V[x] = V[y];
                    break;
                }
                
                case 0x0001: {
                    V[x] |= V[y];
                    break;
                }
                case 0x0002: {
                    V[x] &= V[y];
                    break;
                }
                case 0x0003: {
                    V[x] ^= V[y];
                    break;
                }
                case 0x0004: {
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }

                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;
                }
                case 0x0005: {
                    if (V[x] > V[y]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }

                    V[x] -= V[y];
                    break;
                }
                case 0x0006: {
                    V[0xF] = V[x] & 0x1U;
	                V[x] >>= 1;
                    break;
                }
                case 0x0007: {
                    if (V[y] > V[x]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    V[x] = V[y] - V[x];
                    break;
                }
                case 0x000E: {
                    V[0xF] = V[x] & 0x80U >> 7U;
	                V[x] <<= 1;
                    break;
                }
                default:
                    exit(-1);
            }
        }
        case 0x9000: {
            if (V[x] != V[y]) {
                PC += 2;
            }
            break;
        }
        case 0xA000: {
            I = nnn;
            break;
        }
        case 0xB000: {
            PC = nnn + V[0x0];
            break;
        }
        case 0xC000: {
            V[x] = randomByte(seed) & kk;
            break;
        }
        case 0xD000: {
            unsigned short height = n;
            unsigned short pixel{};

            V[0xF] = 0;
            for (int yLine = 0; yLine < height; yLine++) { 
                pixel = memory[I + yLine];
                for(int xLine = 0; xLine < 8; xLine++) {
                    if((pixel & (0x80 >> xLine)) != 0) {
                        if(display[(x + xLine + ((y + yLine) * 64))] == 1) {
                            V[0xF] = 1;                                 
                            display[x + xLine + ((y + yLine) * 64)] ^= 1;
                        }
                    }
                }
            }         
            drawFlag = true;
            PC += 2;
            break;
        }
        case 0xE000: {
            switch (opcode & 0x00FF) {
                case 0x009E: {
                    if (keys[V[x]]) {
                        PC += 2;
                    }
                    break;
                }
                case 0x00A1: {
                    if (!keys[V[y]]) {
                        PC += 2;
                    }
                    break;
                }
                default:
                    exit(-1);
            }
            break;
        }
        case 0xF000: 
            switch (opcode & 0x00FF) {
                case 0x0007: {
                    V[x] = delayTimer;
                    break;
                }
                case 0x000A: {
                    bool isKeyPressed{false};
                    for (int i{0}; i < 16; ++i) {
                        if (keys[i] != 0) {
                            V[x] = I;
                            isKeyPressed = true;
                        }
                    }
                    if (!isKeyPressed) {
                        return;
                    }
                    PC+=2;
                    break;
                }
                case 0x0015: {
                    delayTimer = V[x];
                    break;
                }
                case 0x0018: {
                    soundTimer = V[x];
                    break;
                }
                case 0x001E: {
                    if (I + V[x] > 0xFFF) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    I += V[x];
                    break;
                }
                case 0x0029: {
                    I = V[x] * 0x5;
                    break;
                }
                case 0x0033: {
                    memory[I] = V[x] / 100;
                    memory[I + 1] = (V[x] / 10) % 10;
                    memory[I + 2] = (V[x] % 100) % 10;
                    PC += 2;
                    break;
                }
                case 0x0055: {
                    for (int i{0}; i <= x; ++i) {
                        memory[I + i] = V[i];
                    }
                    break;
                }
                case 0x0065: {
                    for (int i{0}; i <= x; ++i) {
                        V[i] = memory[I + i];
                    }
                    break;
                }
                default:
                    exit(-1);
            }
            break;
        default:
            exit(-1);
    } 
}

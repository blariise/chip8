#ifndef CHIP8_H
#define CHIP8_H

typedef struct Chip8 {
  std::array<uint8_t, 4096> memory;  // memory of 4kB
  std::array<uint8_t, 16> V;     // cpu register from V0 - VF
  uint8_t SP; // level of the stack (stack pointer)

	uint16_t PC; // pointer to current instruction in memory (program counter)
	uint16_t I; // pointer to location in memory (index register)

  std::array<uint8_t, 64 * 32> display; // display 64x32 px
	uint8_t delay_timer;
	uint8_t sound_timer;

	/* 
		std::default_random_engine seed;
		std::uniform_int_distribution<unsigned char> randomByte;
	*/

	Chip8 init(); // Initializing and reseting machine
	void cycle(); // Fetch, Decode, Execute (One cycle)
	void loadRom(char const* filename);
} Chip8;
#endif // CHIP_8_H


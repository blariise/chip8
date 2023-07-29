#pragma once

struct Chip8 {
	private:
		uint8_t memory[4096]{};  // memory of 4kB
		uint8_t V[16]{};     // cpu register from V0 - VF
		uint8_t SP{}; // level of the stack (stack pointer)

		uint16_t PC{}; // pointer to current instruction in memory (program counter)
		uint16_t I{}; // pointer to location in memory (index register)

		uint8_t display[64 * 32]{}; // display 64x32 px
		uint8_t delayTimer{};
		uint8_t soundTimer{};

		/* 
		void init();
		std::default_random_engine seed;
		std::uniform_int_distribution<unsigned char> randomByte;
		*/

		void cycle(); // Fetch, Decode, Execute (One cycle)
		void loadRom(char const *filename);
};
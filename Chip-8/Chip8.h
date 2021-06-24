#ifndef CHIP8_H
#define CHIP8_H

struct Chip8 {
	private:
		unsigned char memory[4096]{};  // memory of 4kB
		unsigned char V[16]{};     // cpu register from V0 - VF
		unsigned char stack[16]{};   // current location before a next instruction

		unsigned short PC{}; // pointer to current instruction in memory (program counter)
		unsigned short I{}; // pointer to location in memory (index register)
		unsigned short opcode; // current opcode
		unsigned short SP{}; // level of the stack (stack pointer)

		unsigned char delayTimer{};
		unsigned char soundTimer{};

		void init(){};

	public:
		unsigned char display[64][32]{}; // display 64x32 px
		unsigned char keys[16]{}; // keypad
		
		Chip8();
		~Chip8();

		void cycle(); // Fetch, Decode, Execute (One cycle)
		void loadRom(char const *filename);
};
#endif
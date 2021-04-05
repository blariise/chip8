package chip8

type Vm struct {
	memory  [4096]uint8   // memory of 4kB
	display [64][32]uint8 // display size 64x32 px
	V       [16]uint8     // cpu register from V0 - VF
	stack   [16]uint16    // current location before a next instruction

	pc     uint16 // pointer to current instruction in memory (program counter)
	I      uint16 // pointer to location in memory (index register)
	opcode uint16 // current opcode
	sp     uint16 // level of the stack (stack pointer)

	delayTimer uint8
	soundTimer uint8
}

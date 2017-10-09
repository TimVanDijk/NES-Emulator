#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include "Memory.h"
#include "Globals.h"

struct Status {
	Bit N : 1;	// negative flag
	Bit V : 1;	// overflow flag
	Bit _ : 1;	// unused (always 1)
	Bit B : 1;	// indicated if BRK was executed
	Bit D : 1;	// decimal mode (ignored)
	Bit I : 1;	// interrupt disable
	Bit Z : 1;	// zero flag
	Bit C : 1;	// carry flag
};

struct Register {
	Byte A;		// accumulator
	Byte X;		// index register X
	Byte Y;		// index register Y
	Status P;	// status register;
	Byte SP;	// stack pointer
	Word PC;	// program counter
};

class CPU {
public:	
	CPU();
	CPU(Memory memory);

	Register regs;

	void step();

private:
	Memory memory;
};
#endif
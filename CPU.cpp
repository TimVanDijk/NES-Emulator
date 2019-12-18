#include "CPU.h"

#include <cstdio>
#include <iostream>
using namespace std;

#define TODO printf("Not implemented yet.\n")

CPU::CPU() { }

CPU::CPU(Memory* memory) {
	this->memory = memory;
	P = {0, 0, 1, 0, 0, 1, 0, 0};
	
	A = 0x00;
	X = 0x00;
	Y = 0x00;

	memory->write(0x4017, 0x00);
	memory->write(0x4015, 0x00);

	for (Word addr = 0x4000; addr <= 0x400F; addr++) {
		memory->write(addr, 0x00);
	}

	for (Word addr = 0x4010; addr <= 0x4013; addr++) {
		memory->write(addr, 0x00);
	}

	PC = memory->read16(0xFFFC);
	SP = 0xFD;
}

void CPU::step() {
	// Fetch
	Byte opcode = memory->read(PC++);

	// Decode
	Instr instr = InstrTable[opcode];
	
	//Execute
	Word data = instr.addr();
	instr.code(data);
}

void CPU::reset() {
	P.I = 1;
	memory->write(0x4015, 0);

	PC = memory->read16(0xFFFC);
	SP = 0xFD;
}

#ifndef NESEMU_MEMORY_H
#define NESEMU_MEMORY_H

#include "Globals.h"

class Memory {
public:	
	Memory();

	Byte read(Address addr);
	void write(Address addr);

private:
	Byte ram [0x800];		// 0x0000-0x0799 2KB internal RAM
	Byte ppuRegs [0x8];		// 0x2000-0x2007 registers exposed by PPU
	Byte ioRegs [0x20];		// 0x4000-0x401F APU and I/O registers
	Byte expRom [0x1FDF];	// 0x4020-0x5FFF expansion ROM
	Byte sram [0x2000];		// 0x6000-0x7FFF SRAM
	Byte prgRomA [0x4000];	// 0x8000-0xBFFF PRG-ROM
	Byte prgRomB [0x4000];	// 0xC000-0xFFFF PRG-ROM

	Byte* translate(Address addr);
};
#endif
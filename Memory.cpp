#include "Memory.h"

Memory::Memory() { }

Byte* Memory::translate(Address addr) {
	if (addr < 0x2000) {
		return &ram[addr % 0x800];
	}
	if (addr < 0x4000) {
		return &ppuRegs[0x2000 + (addr % 0x8)];
	}
	if (addr < 0x4020) {
		return &ioRegs[addr - 0x4000];
	}
	if (addr < 0x6000) {
		return &expRom[addr - 0x4020];
	}
	if (addr < 0x8000) {
		return &sram[addr - 0x6000];
	}
	if (addr < 0xC000) {
		return &prgRomA[addr - 0x8000];
	}
	return &prgRomB[addr - 0xC000];
}

Byte Memory::read(Address addr) {
	return *translate(addr);
}

void Memory::write(Address addr, Byte value) {
	if (addr < 0x4020 || (addr >= 0x6000 && addr < 0x8000)) {
		*translate(addr) = value;
	}
}
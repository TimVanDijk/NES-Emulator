#include "Memory.h"
#include <iostream>
#include <cstdio>
using namespace std;
Memory::Memory() { 
	reset();
}

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

Word Memory::read16(Address addr) {
	Word low = (Word)read(addr);
	Word high = (Word)read(addr + 1);
	return  high << 8 | low;
}

void Memory::write(Address addr, Byte value) {
	if (addr < 0x4020 || (addr >= 0x6000 && addr < 0x8000)) {
		*translate(addr) = value;
	}
}

void Memory::reset() {
	memset(ram, 	0xFF, 0x0800);
	memset(expRom, 	0x00, 0x1FDF);
	memset(sram, 	0x00, 0x2000);
	memset(prgRomA, 0x00, 0x4000);
	memset(prgRomB, 0x00, 0x4000);
}

void Memory::setPrgRomA(Byte* data) {
	memcpy(prgRomA, data, 0x4000);
}

void Memory::setPrgRomB(Byte* data) {
	memcpy(prgRomB, data, 0x4000);
}
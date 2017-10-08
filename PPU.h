#ifndef NESEMU_PPU_H
#define NESEMU_PPU_H

#include "Globals.h"
#include "Memory.h"

class PPU {
public:	
	PPU();
	PPU(Memory mmuMemory);

	void step();

private:
	Memory mmuMemory;
};
#endif
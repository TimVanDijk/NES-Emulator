#ifndef NESEMU_PPU_H
#define NESEMU_PPU_H

#include "Cartridge.h"
#include "Globals.h"
#include "Memory.h"

#include <cstddef>

class PPU {
public:	
	PPU();
	PPU(Memory* mmuMemory);

	void step();

private:
	Memory* mmuMemory;
};
#endif
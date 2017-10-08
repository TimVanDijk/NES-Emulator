#ifndef NESEMU_NES_H
#define NESEMU_NES_H

#include <stdlib.h>
#include <stdint.h>

#include "APU.h"
#include "Cartridge.h"
#include "CPU.h"
#include "GUI.h"
#include "MMU.h"
#include "Memory.h"
#include "PPU.h"

class NES {
public:	
	NES(char* filepath, int width, int height);

	bool isRunning;

	void run();
	void destroy();

private:
	Cartridge cartridge;

	Memory cpuMemory;
	Memory ppuMemory;

	CPU cpu;
	PPU ppu;
	//APU apu;

	//MMU mmu;

	GUI gui;

	bool loadCartridge(Cartridge cartridge);
};
#endif

#include "NES.h"

using namespace std;

NES::NES(char* filepath, int width, int height) {
	cartridge = Cartridge(filepath);

	cpuMemory = Memory();
	ppuMemory = Memory();

	cpu = CPU(cpuMemory);
	ppu = PPU(ppuMemory);
	//apu = apu.initialize();

	//mmu = mmu.initialize();

	gui = GUI(width, height);
}


bool NES::loadCartridge(Cartridge cartridge) {
	Byte* program = cartridge.romData;
	program--;
	//Copy to memory;
	return true;
}

void NES::run() {
	while (isRunning) {
		// TODO: handle interrupts
		cpu.step();

		//On an NTSC system, there are exactly three PPU ticks per CPU cycle
		ppu.step();
		ppu.step();
		ppu.step();
	}
}

void NES::destroy() {
	//if (&cartridge != NULL) free(&cartridge);
	//if (&cpuMemory != NULL) free(&cpuMemory);
	//if (&ppuMemory != NULL) free(&ppuMemory);
	//if (&cpu != NULL) free(&cpu);
	//if (&ppu != NULL) free(&ppu);
	//if (&apu != NULL) free(&apu);
	//if (&mmu != NULL) free(&mmu);
	//if (&gui != NULL) free(&gui);
	//free(this);
}
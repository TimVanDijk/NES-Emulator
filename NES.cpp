#include "NES.h"

#include <iostream>
#include <cstdio>

using namespace std;

NES::NES(char* filepath, int width, int height) {
	memory = new Memory();
	cartridge = new Cartridge(filepath, memory);
	cout << "Cartridge uses mapper #" << int(cartridge->getMapperNr()) << endl;

	cpu = new CPU(memory);
	ppu = new PPU(memory);
	//apu = apu.initialize();
	//mmu = mmu.initialize();

	gui = new GUI(width, height);

	isRunning = true;
}

void NES::run() {
	int i = 0;
	while (isRunning) {
		cpu->printState();
		fflush(stdout);
		// TODO: handle interrupts
		cpu->step();

		//On an NTSC system, there are exactly three PPU ticks per CPU cycle
		ppu->step();
		ppu->step();
		ppu->step();

		i++;
		if (i == 8991)
			return;
	}
}

void NES::reset() {
	cpu->reset();
	memory->reset();

	isRunning = true;

}

void NES::destroy() {
	if (cartridge 	!= NULL) delete cartridge;
	if (memory 		!= NULL) delete memory;
	if (cpu 		!= NULL) delete cpu;
	if (ppu 		!= NULL) delete ppu;
	//if (&apu != NULL) free(&apu);
	//if (&mmu != NULL) free(&mmu);
	if (gui 		!= NULL) delete gui;
	delete this;
}
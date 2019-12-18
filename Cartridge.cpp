#include "Cartridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

Cartridge::Cartridge() { }

Cartridge::Cartridge(char* path, Memory* memory) {
	this->memory = memory;

	loadRom(path);
	parseRom();
}

void Cartridge::loadRom(char* path) {
	// Open romFile.
	FILE* romFile = fopen(path, "rb");
	if (romFile == NULL) {
		cout << "Failed to open " << path << endl;
		exit(1);
	}

	// Allocate memory to write romFile's content to.
	fseek(romFile, 0, SEEK_END);
	uint32_t size = ftell(romFile);
	rewind(romFile);

	romData = (Byte *) calloc(size, sizeof(Byte));
	if (romData == NULL) {
		cout << "Failed to allocate memory to write romFile's content to." << endl;
		exit(1);
	}

	// Copy romFile's content to romData.
	if (size != fread((void *) romData, 1, size, romFile)) {
		cout << "Failed to copy romFile's content to romData." << endl;
		exit(1);
	}

	// Close romFile.
	fclose(romFile);
}

void Cartridge::parseRom() {
	// Check magic
	if (!(romData[0] == 'N' && romData[1] == 'E' && romData[2] == 'S' && romData[3] == 0x1A)) {
		cout << "Magic invalid." << endl;
		exit(1);
	}

	// Parse remainder of header
	prgSize = romData[4] * 0x4000;
	chrSize = romData[5] * 0x2000;

	prgData = romData + 16;
	chrData = prgData + prgSize;

	mapperNr = (romData[7] & 0xF0) | ((romData[6] & 0xF0) >> 4);

	region = romData[10] & 0x3;

	memory->setPrgRomA(prgData);
	memory->setPrgRomB(prgData);
}
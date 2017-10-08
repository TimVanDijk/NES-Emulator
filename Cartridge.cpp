#include "Cartridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Globals.h"

using namespace std;

Cartridge::Cartridge() { }

Cartridge::Cartridge(char* path) {
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
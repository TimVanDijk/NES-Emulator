#ifndef NESEMU_CARTRIDGE_H
#define NESEMU_CARTRIDGE_H

#include "Globals.h"

#include "Memory.h"

class Cartridge {
public:	
	Cartridge();
	Cartridge(char* path, Memory* memory);
	
	Byte getMapperNr() {
		return this->mapperNr;
	}

private:
	Memory* memory;

	Byte* romData;

	Byte* prgData;
	Byte* chrData;

	uint32_t prgSize;
	uint32_t chrSize;

	Byte mapperNr;
	Byte region;

	void loadRom(char* path);
	void parseRom();
};
#endif
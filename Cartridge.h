#ifndef NESEMU_CARTRIDGE_H
#define NESEMU_CARTRIDGE_H

#include <stdint.h>

class Cartridge {
public:	
	Cartridge();
	Cartridge(char* path);
	uint8_t* romData;

private:
	char* path;
};
#endif
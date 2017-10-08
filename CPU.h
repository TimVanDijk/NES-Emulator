#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include "Memory.h"
#include "Globals.h"

class CPU {
public:	
	CPU();
	CPU(Memory cpuMemory);

	void step();

private:
	Memory cpuMemory;
};
#endif
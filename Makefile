OBJS	= emulator.o	APU.o Cartridge.o CPU.o Globals.o GUI.o Memory.o MMU.o NES.o PPU.o
SOURCE	= emulator.cpp	APU.cpp Cartridge.cpp CPU.cpp GUI.cpp Globals.cpp Memory.cpp MMU.cpp NES.cpp PPU.cpp
HEADER	= APU.h Cartridge.h CPU.h Globals.h GUI.h Memory.h NES.h PPU.h
OUT		= NES-emulator
CC		= g++
COMPILER_FLAGS	= -std=c++11 -pthread -W -Wall -c -O3
LINKER_FLAGS	= -lSDL2

test:
	make build;
	./NES-emulator roms/nestest.nes > tests/output.log;
	python3 tests/compare.py

build:
	$(CC) $(COMPILER_FLAGS) $(SOURCE)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OUT)

run:
	./NES-emulator roms/zelda.nes

clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

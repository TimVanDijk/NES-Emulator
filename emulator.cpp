#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "NES.h"

#define DEFAULT_RESOLUTION_WIDTH 256
#define DEFAULT_RESOLUTION_HEIGHT 240

#define BITS_PER_PIXEL 32

using namespace std;

int main(int argc, char** argv) {
	if (argc != 2 && argc != 4) {
		cout << "Usage: ./NES_Emulator <ROM> [<width> <height>]" << endl;
		return 1;
	}
	char* filepath = argv[1];
	
	int width  = (argc == 4) ? atoi(argv[2]) : DEFAULT_RESOLUTION_WIDTH;
	int height = (argc == 4) ? atoi(argv[3]) : DEFAULT_RESOLUTION_HEIGHT;	

	NES* nes = new NES(filepath, width, height);
	nes->run();
	
	nes->destroy();

	return 0;
}
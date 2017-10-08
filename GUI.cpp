#include "GUI.h"

#include "Globals.h"

#define BITS_PER_PIXEL 32

GUI::GUI() { }

GUI::GUI(int width, int height) {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Surface* logoSurface = NULL;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if(window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	screenSurface = SDL_GetWindowSurface(window);
	logoSurface = SDL_LoadBMP("logo.bmp");
	if (logoSurface == NULL) {
		printf("Logo could not be loaded! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_BlitSurface( logoSurface, NULL, screenSurface, NULL );
	SDL_UpdateWindowSurface(window);

	SDL_Delay(2000);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
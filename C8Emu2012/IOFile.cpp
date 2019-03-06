#include <time.h>
#include <stdlib.h>
#include <SDL.h>
#include "IOFile.h"

static unsigned char Display[64][32];
static SDL_Rect Pixels[64][32];
static SDL_Renderer* renderer;

void SetDelayTimer(unsigned char time) {

}
void SetSoundTimer(unsigned char time) {

}
unsigned char GetDelayTimer() {

}
unsigned char GetSoundTimer() {

}

unsigned char GetRand() {
	return (rand() % 256);
}

unsigned char SetPixel(unsigned char x, unsigned char y, unsigned char bit) {
	unsigned char previous = Display[x][y];
	Display[x][y] ^= bit;
	return previous;
}

void ClearDisplay() {

}

void IOInit() {
	srand(time(0));
	int scale = 20;
	int current_x = 0;
	int current_y = 0;

	SDL_Window* window;

	window = NULL;
	window = SDL_CreateWindow("C8LLVM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * scale, 32 * scale, SDL_WINDOW_SHOWN);

	// Setup renderer
	renderer = NULL;
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	// Creat a rect at pos ( i, j ) that's 10 pixels wide and 10 pixels high.

	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 32; ++j) {
			Pixels[i][j].x = i * scale;
			Pixels[i][j].y = j * scale;
			Pixels[i][j].w = scale;
			Pixels[i][j].h = scale;
		}
	}
}

void IOLoop() {
	// Set render color to red ( background will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear winow
	SDL_RenderClear(renderer);

	// Set render color to blue ( rect will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 32; ++j) {
			if (Display[i][j] != 0) {
				// Render rect
				SDL_RenderFillRect(renderer, &Pixels[i][j]);
			}
		}
	}

	// Render the rect to the screen
	SDL_RenderPresent(renderer);
}

bool GetKeyPress() {

}
bool WaitOnKeyPress() {

}
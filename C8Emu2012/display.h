#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include<SDL.h>
#else
#include<SDL2/SDL.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>

std::string NAME;

class Display {
	int scale = 20;
	int current_x = 0;
	int current_y = 0;
	SDL_Event e;


	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Rect PIXEL[64][32];
	//bool PRI_DISPLAY[64][32];

public:
	 Display()
	{
		window = NULL;
		window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * scale, 32 * scale, SDL_WINDOW_SHOWN);

		// Setup renderer
		renderer = NULL;
		renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

		// Creat a rect at pos ( i, j ) that's 10 pixels wide and 10 pixels high.

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{
				PIXEL[i][j].x = i * scale;
				PIXEL[i][j].y = j * scale;
				PIXEL[i][j].w = scale;
				PIXEL[i][j].h = scale;
				//DISPLAY[i][j] = false;
			}
		}

	}

	 int SDL_Loop()
	 {

		 // Set render color to red ( background will be rendered in this color )
		 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		 // Clear winow
		 SDL_RenderClear(renderer);

		 // Set render color to blue ( rect will be rendered in this color )
		 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		 for (int i = 0; i < 64; ++i)
		 {
			 for (int j = 0; j < 32; ++j)
			 {
				 if (DISPLAY[i][j] != 0)
				 {
					 // Render rect
					 SDL_RenderFillRect(renderer, &PIXEL[i][j]);
				 }
			 }
		 }

		 // Render the rect to the screen
		 SDL_RenderPresent(renderer);

		 return 0;
	 }

	 void Keys(bool &quit)
	 {

		 //Process User input
		 while (SDL_PollEvent(&e) != 0)
		 {
			 //User requests quit
			 if (e.type == SDL_QUIT)
			 {
				 quit = true;
			 }
			 if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			 {
				 //Select surfaces based on key press
				 switch (e.key.keysym.sym)
				 {
				 case SDLK_0:
					 if (e.type == SDL_KEYDOWN)
					 {
						 std::ofstream file;
						 file.open(NAME + "_MemoryTrace.txt");
						 std::stringstream stream;
						 for (Usage u : MEMORY_USAGE) {
							 stream << u << "\n";
						 }
						 file << stream.str();
						 file.close();
						 quit = true;
					 }
					 break;

				 case SDLK_1:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[1] = true;
					 }
					 else
					 {
						 KEYS[1] = false;
					 }
					 break;

				 case SDLK_2:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[2] = true;
					 }
					 else
					 {
						 KEYS[2] = false;
					 }
					 break;

				 case SDLK_3:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[3] = true;
					 }
					 else
					 {
						 KEYS[3] = false;
					 }
					 break;

				 case SDLK_4:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[12] = true;
					 }
					 else
					 {
						 KEYS[12] = false;
					 }
					 break;

				 case SDLK_q:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[4] = true;
					 }
					 else
					 {
						 KEYS[4] = false;
					 }
					 break;

				 case SDLK_w:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[5] = true;
					 }
					 else
					 {
						 KEYS[5] = false;
					 }
					 break;

				 case SDLK_e:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[6] = true;
					 }
					 else
					 {
						 KEYS[6] = false;
					 }
					 break;

				 case SDLK_r:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[13] = true;
					 }
					 else
					 {
						 KEYS[13] = false;
					 }
					 break;

				 case SDLK_a:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[7] = true;
					 }
					 else
					 {
						 KEYS[7] = false;
					 }
					 break;

				 case SDLK_s:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[8] = true;
					 }
					 else
					 {
						 KEYS[8] = false;
					 }
					 break;

				 case SDLK_d:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[9] = true;
					 }
					 else
					 {
						 KEYS[9] = false;
					 }
					 break;

				 case SDLK_f:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[14] = true;
					 }
					 else
					 {
						 KEYS[14] = false;
					 }
					 break;

				 case SDLK_z:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[10] = true;
					 }
					 else
					 {
						 KEYS[10] = false;
					 }
					 break;

				 case SDLK_x:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[0] = true;
					 }
					 else
					 {
						 KEYS[0] = false;
					 }
					 break;

				 case SDLK_c:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[11] = true;
					 }
					 else
					 {
						 KEYS[11] = false;
					 }
					 break;

				 case SDLK_v:
					 if (e.type == SDL_KEYDOWN)
					 {
						 KEYS[15] = true;
					 }
					 else
					 {
						 KEYS[15] = false;
					 }
					 break;

				 default:
					 break;
				 }
			 }
		 }
	 }

	 ~Display()
	 {
		 SDL_DestroyWindow(window);
		 SDL_Quit();
	 }

		
};

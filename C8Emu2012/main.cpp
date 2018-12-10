#include "main.h"
#include "memreg.h"
#include "display.h"
#include <time.h>
//#include <iomanip> //used for debug

bool quit = false;
Uint32 START_TIME = 0;
Uint32 END_TIME = 0;
bool Tick = true;
//long long frequency;


int SDL_Stuff();

int Error(char ERROR_MESSAGE[])
{
	std::cout << ERROR_MESSAGE << std::endl;
	system("pause");
	return -1;
}

int OpenFile(int &argc, char *argv[])
{
	if (argc <= 1) //Checks if File was specified
	{
		return Error("No file");
	}



	else //If one was...
	{
		NAME = argv[1];
		FILE *FILESTREAM = fopen(argv[1], "rb");	// Open in read binary
		if (FILESTREAM)								// If successful...
		{
			fseek(FILESTREAM, 0, SEEK_END);				//Finding File Length: goes to the end, counts Bytes from origin and comes back
			long FILE_SIZE = ftell(FILESTREAM);
			fseek(FILESTREAM, 0, SEEK_SET);

			fread((MEMORY + 0x200), 1, FILE_SIZE, FILESTREAM); //Adds BYTES to memory starting at 0x200



			if (ferror(FILESTREAM))
			{
				return Error("File not extracted to memory properly");
			}



			fclose(FILESTREAM);

		}
		else
		{
			return Error("File cannot be opened");
		}

	}
	return 0;
}

int Game_Cycle()
{
	
	//Check Keys SDL Stuff

	return 0;
}

int main(int argc, char *argv[])
{
	
	Init();
	Display display;

	if(OpenFile(argc, argv) == 0)
	{
		while (!quit)
		{
			START_TIME = SDL_GetTicks() - END_TIME;

			display.Keys(quit);

			if (Tick)
			{
				SDL_GetPerformanceCounter();

				if (MEMORY_USAGE[PC] == Data) {
					MEMORY_USAGE[PC] = Both;
				} else {
					MEMORY_USAGE[PC] = Code;
				}

				if (MEMORY_USAGE[PC + 1] == Data) {
					MEMORY_USAGE[PC + 1] = Both;
				}
				else {
					MEMORY_USAGE[PC + 1] = Code;
				}
				WORD = (MEMORY[PC] << 8) | (MEMORY[PC + 1]);

				OP_Function();

				if (DRAW)
				{
					display.SDL_Loop();
					DRAW = false;
				}
				Tick = false;
			}

			if (START_TIME > 0.017)
			{
				END_TIME = SDL_GetTicks();
				Tick = true;
			}

		}
	}
	else
	{
		exit(0);
	}


//	for(int i = 0x190; i <= 0x300; ++i)
//	{
//		std::cout << std::hex << i << static_cast<int>(MEMORY[i]) << std::endl;
//	}
	system("pause");
	return 0;
}

int SDL_Stuff()
{

	return 0;
}

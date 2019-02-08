#include "memreg.h"
#include <stdlib.h>
#include <time.h>

#pragma region Memory, Display etc.

unsigned char MEMORY[4096]; //Dear God, there must be a better way than this :( 
//Also note that the CHIP-8 interpreter would use the first 512 BYTES (font data),
//the uppermost 256 are reserved for display refresh,
//and the 96 bytes below that were reserved for call stack, internal use, and other variables.

Usage MEMORY_USAGE[4096];

unsigned short FUNCTION_PC_HEADER[4096];
unsigned short FUNCTION_USAGE_AMT[4096];
unsigned short FUNCTION_PCS[16];

unsigned char REG_V[16]; //unsigned so C++ can interact with stored values properly

unsigned short REG_VI; //the address regester, used for memory access n' stuff

unsigned char TIMER_DELAY; //This timer does nothing more than subtract 1 from the value of DT at a rate of 60Hz.
unsigned char TIMER_SOUND; //as long as ST's value is greater than zero, the Chip-8 buzzer will sound.

//"pseudo-registers"

unsigned short STACK[16];

unsigned char SP;
unsigned short PC;
//bool DF; //draw flag

unsigned short WORD; //opcode reader

//Keyboard

bool KEYS[16];

//Display

bool DRAW;

bool DISPLAY[64][32];

//unsigned short MEM_FONT[16];

//The original implementation of the Chip - 8 language used a 64x32 - pixel monochrome display with this format:

//(0, 0)	(63, 0)
//(0, 31)	(63, 31)

//Some other interpreters, most notably the one on the ETI 660, also had 64x48 and 64x64 modes.
//To my knowledge, no current interpreter supports these modes.More recently,
//Super Chip - 48, an interpreter for the HP48 calculator, added a 128x64 - pixel mode.
//This mode is now supported by most of the interpreters on other platforms.

#pragma endregion

void Fontset_Init();

int Init()
{
	srand(time(NULL));
	for (unsigned int i = 0; i < 4096; ++i) // sets memory to 0
	{
		MEMORY[i] = 0;
		MEMORY_USAGE[i] = Unused;
		FUNCTION_PC_HEADER[i] = 0;
		FUNCTION_USAGE_AMT[i] = 0;
	}
	for (unsigned char i = 0; i < 16; ++i)
	{
		REG_V[i] = 0;
		STACK[i] = 0;
		FUNCTION_PCS[i] = 0;
		KEYS[i] = false;
	}

	REG_VI = 0;

	TIMER_DELAY = 0;
	TIMER_SOUND = 0;

	SP = 0;
	PC = 0x200; //beginning of program

	WORD = 0; //opcode reader

	Fontset_Init();

	for (unsigned char i = 0; i < 64; ++i)
	{
		for (unsigned char j = 0; j < 32; ++j)
		{
			DISPLAY[i][j] = 0;
		}
	}
	//Init SDL and OpenGL too

	DRAW = false;

	return 0;
}

unsigned char Fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

inline void Fontset_Init()
{
	for (unsigned char i = 0; i < 80; ++i)
	{
		MEMORY[i] = Fontset[i];
	}
}
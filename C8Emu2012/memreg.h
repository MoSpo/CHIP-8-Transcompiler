#pragma region Memory, Display etc.

extern unsigned char MEMORY[4096]; //Dear God, there must be a better way than this :( 
//Also note that the CHIP-8 interpreter would use the first 512 BYTES (font data),
//the uppermost 256 are reserved for display refresh,
//and the 96 bytes below that were reserved for call stack, internal use, and other variables.

enum Usage { Unused, Data, Code, Both };

extern Usage MEMORY_USAGE[4096];

extern unsigned short FUNCTION_PC_HEADER[4096];
extern unsigned short FUNCTION_USAGE_AMT[4096];
extern unsigned short FUNCTION_PCS[16];

extern unsigned char REG_V[16]; //unsigned so C++ can interact with stored values properly

extern unsigned short REG_VI; //the address regester, used for memory access n' stuff

extern unsigned char TIMER_DELAY; //This timer does nothing more than subtract 1 from the value of DT at a rate of 60Hz.
extern unsigned char TIMER_SOUND; //as long as ST's value is greater than zero, the Chip-8 buzzer will sound.

//"pseudo-registers"

extern unsigned short STACK[16];

extern unsigned char SP;
extern unsigned short PC;
//extern bool DF; //draw flag

extern unsigned short WORD; //opcode reader

//Keyboard

extern bool KEYS[16];

//Display

extern bool DISPLAY[64][32];

extern bool DRAW;

//The original implementation of the Chip - 8 language used a 64x32 - pixel monochrome display with this format:

//(0, 0)	(63, 0)
//(0, 31)	(63, 31)

//Some other interpreters, most notably the one on the ETI 660, also had 64x48 and 64x64 modes.
//To my knowledge, no current interpreter supports these modes.More recently,
//Super Chip - 48, an interpreter for the HP48 calculator, added a 128x64 - pixel mode.
//This mode is now supported by most of the interpreters on other platforms.

#pragma endregion

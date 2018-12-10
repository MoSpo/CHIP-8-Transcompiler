#include <iomanip>
#include "memreg.h"
#include <iostream>

int Error(char ERROR_MESSAGE[]);

bool EMU_STOP = false;

bool OLD_KEYS[16];

	void OP_NULL()
	{
		//Function Array Pointer Padding
	}

#pragma region _3rd_level

	void OP_FX15() //TIMER_DELAY is set equal to the value of VX.
	{
		TIMER_DELAY = REG_V[(WORD & 0x0F00) >> 8];
	}
	void OP_FX18() //TIMER_SOUND is set equal to the value of VX.
	{
		TIMER_SOUND = REG_V[(WORD & 0x0F00) >> 8];
	}
	void OP_FX1E() //The values of VI and VX are added, and the results are stored in VI.
	{
		REG_VI = REG_VI + REG_V[(WORD & 0x0F00) >> 8]; //VF is set to 1 when range overflow (I+VX>0xFFF), and 0 when there isn't. This is undocumented feature of the CHIP-8 and used by Spacefight 2091! game.
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	void(*OP_Table_FX1_[16])() =
	{
		OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_FX15, OP_NULL, OP_NULL, OP_FX18, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_FX1E, OP_NULL
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region _2nd_level

	void OP_8XY0() //Stores the value of register VY in register VX.
	{
		REG_V[(WORD & 0x0F00) >> 8] = REG_V[(WORD & 0x00F0) >> 4];
	}
	void OP_8XY1() //Performs a bitwise OR on the values of VX and VY, then stores the result in VX.
	{
		REG_V[(WORD & 0x0F00) >> 8] |= REG_V[(WORD & 0x00F0) >> 4];
	}
	void OP_8XY2() //Performs a bitwise AND on the values of VX and VY, then stores the result in VX.
	{
		REG_V[(WORD & 0x0F00) >> 8] &= REG_V[(WORD & 0x00F0) >> 4];
	}
	void OP_8XY3() //Performs a bitwise exclusive OR on the values of VX and VY, then stores the result in VX.
	{
		REG_V[(WORD & 0x0F00) >> 8] ^= REG_V[(WORD & 0x00F0) >> 4];
	}
	void OP_8XY4() //The values of VX and VY are added together. If the result is greater than 8 bits VF is set to 1, otherwise 0. Lowest 8 bits are stored in VX.
	{
		unsigned int TEMP_VAR = REG_V[(WORD & 0x0F00) >> 8] + REG_V[(WORD & 0x00F0) >> 4];
		if (TEMP_VAR > 0xFF)
		{
			REG_V[15] = 1;
		}
		else
		{
			REG_V[15] = 0;
		}
		REG_V[(WORD & 0x0F00) >> 8] = (TEMP_VAR & 0xFF);
	}
	void OP_8XY5() //If VX > VY, then VF is set to 1, otherwise 0. Then VY is subtracted from VX, and the results stored in VX.
	{
		unsigned int TEMP_VAR = 0xF00 + REG_V[(WORD & 0x0F00) >> 8] - REG_V[(WORD & 0x00F0) >> 4]; //add 0xF00 as something to borrow from.
		if (TEMP_VAR > 0xF00)
		{
			REG_V[15] = 1;
		}
		else
		{
			REG_V[15] = 0;
		}
		REG_V[(WORD & 0x0F00) >> 8] = (TEMP_VAR & 0xFF);
	}
	void OP_8XY6() //If the least-significant bit of VX is 1, then VF is set to 1, otherwise 0. Then VX is divided by 2.
	{
		if ((REG_V[(WORD & 0x0F00) >> 8] & 0x01) == 0x01)
		{
			REG_V[15] = 1;
		}
		else //On the original interpreter, the value of VY is shifted, and the result is stored into VX. On current implementations, Y is ignored.
		{
			REG_V[15] = 0;
		}
		REG_V[(WORD & 0x0F00) >> 8] >>= 1; 
	}
	void OP_8XY7()//If VY > VX, then VF is set to 1, otherwise 0. Then VX is subtracted from VY, and the results stored in VX.
	{
		unsigned int TEMP_VAR = 0xF00 + REG_V[(WORD & 0x00F0) >> 4] - REG_V[(WORD & 0x0F00) >> 8]; //add 0xF00 as something to borrow from.
		if (TEMP_VAR > 0xF00)
		{
			REG_V[15] = 1;
		}
		else
		{
			REG_V[15] = 0;
		}
		REG_V[(WORD & 0x0F00) >> 8] = (TEMP_VAR & 0xFF);
	}
	void OP_8XYE()//If the most-significant bit of VX is 1, then VF is set to 1, otherwise to 0. Then VX is multiplied by 2.
	{
		if ((REG_V[(WORD & 0x0F00) >> 8] & 0x80) == 0x80)
		{
			REG_V[15] = 1;
		}
		else //On the original interpreter, the value of VY is shifted, and the result is stored into VX. On current implementations, Y is ignored.
		{
			REG_V[15] = 0;
		}
		REG_V[(WORD & 0x0F00) >> 8] <<= 1;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	void(*OP_Table_8XY_[16])() =
	{
		OP_8XY0, OP_8XY1, OP_8XY2, OP_8XY3, OP_8XY4, OP_8XY5, OP_8XY6, OP_8XY7, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_8XYE, OP_NULL
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////
	void OP_FX0_()
	{
		if ((WORD & 0x000F) == 0x0007) //OP_FX07 //The value of TIMER_DELAY is placed into VX.
		{
			REG_V[(WORD & 0x0F00) >> 8] = TIMER_DELAY;
		}
		else //OP_FX0A //Wait for a key press, store the value of the key in Vx. All execution stops until a key is pressed.
		{
			// ALL EXECUTION STOPS UNTIL KEY IS PRESSED

			if (!EMU_STOP)
			{
				EMU_STOP = true;
				for (unsigned char i = 0; i < 16; ++i)
				{
					OLD_KEYS[i] = KEYS[i];
				}
			}
			else
			{
				for (unsigned char i = 0; i < 16; ++i)
				{
					if ((OLD_KEYS[i] == false) && (KEYS[i] == true))
					{
						REG_V[(WORD & 0x0F00) >> 8] = i;
						EMU_STOP = false;
						break;
					}
					OLD_KEYS[i] = KEYS[i];
				}
			}
		}
	}
	void OP_FX1_()
	{
		(*(OP_Table_FX1_[WORD & 0x000F]))();
	}
	void OP_FX29() //The value of VI is set to the location for the hexadecimal font sprite corresponding to the value of VX.
	{
		REG_VI = REG_V[(WORD & 0x0F00) >> 8] * 5;
	}
	void OP_FX33() //(BCD)takes the decimal value of Vx, and places the hundreds digit at location in I, tens digit at location I+1, ones digit at location I+2.
	{
		unsigned char TEMP_VAR = REG_V[(WORD & 0x0F00) >> 8];
		for (int i = 0; i < 3; i++) {
			if (MEMORY_USAGE[REG_VI + i] == Code) {
				MEMORY_USAGE[REG_VI + i] = Both;
			}
			else {
				MEMORY_USAGE[REG_VI + i] = Data;
			}
		}
		MEMORY[REG_VI] = TEMP_VAR / 100;
		MEMORY[REG_VI + 1] = (TEMP_VAR / 10) % 10;
		MEMORY[REG_VI + 2] = TEMP_VAR % 10;
	}
	void OP_FX55() //The interpreter copies the values of registers V0 through to VX (inclusive)(confirmed) into memory, starting at the address in I.
	{
		for (unsigned char REG_MEM = 0; REG_MEM <= ((WORD & 0x0F00) >> 8); ++REG_MEM)
		{
			if (MEMORY_USAGE[REG_VI + REG_MEM] == Code) {
				MEMORY_USAGE[REG_VI + REG_MEM] = Both;
			}
			else {
				MEMORY_USAGE[REG_VI + REG_MEM] = Data;
			}
			MEMORY[REG_VI + REG_MEM] = REG_V[REG_MEM];  //On the original interpreter, when the operation is done, I = I + X + 1. On current implementations, I is left unchanged.
			//REG_VI += ((WORD & 0x0F00) >> 8) + 1;
		}
	}
	void OP_FX65() //The interpreter reads values from memory starting at location I into registers V0 through Vx.
	{
		for (unsigned char REG_MEM = 0; REG_MEM <= ((WORD & 0x0F00) >> 8); ++REG_MEM)
		{
			if (MEMORY_USAGE[REG_VI + REG_MEM] == Code) {
				MEMORY_USAGE[REG_VI + REG_MEM] = Both;
			}
			else {
				MEMORY_USAGE[REG_VI + REG_MEM] = Data;
			}
			REG_V[REG_MEM] = MEMORY[REG_VI + REG_MEM]; //On the original interpreter, when the operation is done, I = I + X + 1. On current implementations, I is left unchanged.
			//REG_VI += ((WORD & 0x0F00) >> 8) + 1;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	void(*OP_Table_FX__[7])() =
	{
		OP_FX0_, OP_FX1_, OP_FX29, OP_FX33, OP_NULL, OP_FX55, OP_FX65
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region _1st_level
	void OP_0___()
	{
		if ((WORD & 0x0F00) == 0x0000) //Using this as opposed to a function pointer array because I'm hoping it's better in the long run
		{
			if ((WORD & 0x000F) == 0x0000) //OP_00E0 //Clear the display.
			{
				for (unsigned char i = 0; i < 64; ++i)
				{
					for (unsigned char j = 0; j < 32; ++j)
					{
						DISPLAY[i][j] = 0;
					}
				}
				DRAW = true;
			}
			else //OP_00EE //Return from a subroutine. Sets PC to the address at the (top of the stack)(?), then subtracts 1 from the stack pointer.
			{
				--SP;
				PC = STACK[SP];
			}
		}
		else //OP_0NNN //Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
		{
			//I HAVE NO IDEA WHAT THIS DOES
			std::cout << "oh dear" << std::endl;
		}
	}
	void OP_1NNN() //The interpreter sets the program counter to NNN.
	{
		PC = (WORD & 0x0FFF); 
		PC -= 2;
	}
	void OP_2NNN() //The interpreter (increments the stack pointer)(?), then puts the current PC on the top of the stack. The PC is then set to NNN.
	{
		STACK[SP] = PC;
		++SP;
		PC = (WORD & 0x0FFF);
		PC -= 2;
	}
	void OP_3XNN() //Compares register VX to NN, and if they are equal, skips the next command.
	{
		if (REG_V[(WORD & 0x0F00) >> 8] == (WORD & 0x00FF))
		{
			PC += 2;
		}
	}
	void OP_4XNN() //Compares register VX to NN, and if they are NOT equal, skips the next command.
	{
		if (REG_V[(WORD & 0x0F00) >> 8] != (WORD & 0x00FF))
		{
			PC += 2;
		}
	}
	void OP_5XY0() //Compares register VX to VY, and if they are equal, skips the next command.
	{
		if (REG_V[(WORD & 0x0F00) >> 8] == REG_V[(WORD & 0x00F0) >> 4])
		{
			PC += 2;
		}
	}
	void OP_6XNN() //Puts the value NN into register VX.
	{
		REG_V[(WORD & 0x0F00) >> 8] = (WORD & 0x00FF);
	}
	void OP_7XNN() //Adds the value NN to the value of register VX, then stores the result in VX. 
	{
		REG_V[(WORD & 0x0F00) >> 8] += (WORD & 0x00FF);
	}
	void OP_8XY_()
	{
		(*(OP_Table_8XY_[WORD & 0x000F]))();
	}
	void OP_9XY0() //Compares register VX to VY, and if they are NOT equal, skips the next command.
	{
		if (REG_V[(WORD & 0x0F00) >> 8] != REG_V[(WORD & 0x00F0) >> 4])
		{
			PC += 2;
		}
	}
	void OP_ANNN() //The value of VI is set to NNN.
	{
		REG_VI = (WORD & 0x0FFF);
	}
	void OP_BNNN() //The PC is to NNN plus the value of V0.
	{
		PC = (WORD & 0x0FFF) + REG_V[0];
	}
	void OP_CXNN() //generates a random number from 0 to 255, and & with the value NN. The result is stored in VX.
	{
		REG_V[(WORD & 0x0F00) >> 8] = ((WORD & 0x00FF) & (rand() % 256));
	}
	void OP_DXYN() // DRAWS SPRITES TO SCREEN
	{

		REG_V[15] = 0;
		for (unsigned char j = 0; j < (WORD & 0x000F); ++j) //^ onto the screen. If this causes any pixels to be erased, VF is set to 1, otherwise 0.
		{
			
			for (unsigned char i = 0; i < 8; ++i) //If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
			{
				if (MEMORY_USAGE[REG_VI + j] == Code) {
					MEMORY_USAGE[REG_VI + j] = Both;
				}
				else {
					MEMORY_USAGE[REG_VI + j] = Data;
				}

				if (((MEMORY[REG_VI + j] & (128 >> i)) >> (7 - i)) == 1)
				{
					if (DISPLAY[(REG_V[(WORD & 0x0F00) >> 8] + i) % 64][(REG_V[(WORD & 0x00F0) >> 4] + j) % 32] == 1)
					{
						REG_V[15] = 1;
					}
					DISPLAY[(REG_V[(WORD & 0x0F00) >> 8] + i) % 64][(REG_V[(WORD & 0x00F0) >> 4] + j) % 32] ^= ((MEMORY[REG_VI + j] & (128 >> i)) >> (7 - i));
				}
			}
		}
		DRAW = true;

	}
	void OP_EX__()
	{
		if ((WORD & 0x000F) == 0x001) //OP_EXA1 //Skip next instruction if key with the value of VX is not pressed.
		{
			if (KEYS[REG_V[(WORD & 0x0F00) >> 8]] == false)
			{
				PC += 2;
			}
		}
		else //OP_EX9E //Skip next instruction if key with the value of VX is pressed.
		{
			if (KEYS[REG_V[(WORD & 0x0F00) >> 8]] == true)
			{
				PC += 2;
			}
		}

	}
	void OP_FX__()
	{
		(*(OP_Table_FX__[(WORD & 0x00F0) >> 4]))();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	void(*OP_Table_F000[16])() =
	{
		OP_0___, OP_1NNN, OP_2NNN, OP_3XNN, OP_4XNN, OP_5XY0, OP_6XNN, OP_7XNN,
		OP_8XY_, OP_9XY0, OP_ANNN, OP_BNNN, OP_CXNN, OP_DXYN, OP_EX__, OP_FX__
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

	int OP_Function()
	{
		//OP_Table_F000[(WORD & 0xF000) >> 12](); this also implicitly works
		(*(OP_Table_F000[(WORD & 0xF000) >> 12]))();
		//std::cout << std::hex << WORD << std::endl;

	//	std::cout << "hi" << std::endl;

		if (!EMU_STOP)
		{
			PC += 2;

			if (TIMER_DELAY > 0)
			{
				--TIMER_DELAY;
			}

			if (TIMER_SOUND > 0)
			{
				--TIMER_SOUND;
			}
		}
		//Return stuff
		return 0;
	}

#include "Lib.h"
#include "Ast.h"
#ifndef LEXER_H
#define LEXER_H
class Lexer {

private:

	Ast* OP_Eval(const std::string& operationName, const std::string& operationDescription, const std::vector<unsigned int>& operandMasks);
	Ast* OP_NULL();

	Ast* OP_0___(); //"00BX" print V[X] (Debug only), {0x000F}); "00E0", "Clear the display", {}); "00EE", "Return from subroutine", {});

	Ast* OP_1NNN(); //"1NNN", "PC = NNN", { 0x0FFF }); }
	Ast* OP_2NNN(); //"2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
	Ast* OP_3XNN(); //"3XNN", "If V[X] == NNN, skip next instruction", { 0x0F00, 0x00FF }); }
	Ast* OP_4XNN(); //"4XNN", "If V[X] != NNN, skip next instruction", { 0x0F00, 0x00FF }); }
	Ast* OP_5XY0(); //"5XY0", "If V[X] == V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
	Ast* OP_6XNN(); //"6XNN", "V[X] = NN", { 0x0F00, 0x00FF }); }
	Ast* OP_7XNN(); //"7XNN", "V[X] += NN", { 0x0F00, 0x00FF }); }

	Ast* OP_8XY0(); //"8XY0", "V[X] = V[Y]", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XY1(); //"8XY1", "V[X] |= V[Y]", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XY2(); //"8XY2", "V[X] &= V[Y]", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XY3(); //"8XY3", "V[X] ^= V[Y]", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XY4(); //"8XY4", "V[X] += V[Y] w/ VF set in overflow", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XY5(); //"8XY5", "V[X] -= V[Y] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XY6(); //"8XY6", "V[X] >>= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XY7(); //"8XY7", "V[X] = V[Y] - V[X] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
	Ast* OP_8XYE(); //"8XYE", "V[X] <<= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }

	Ast*(Lexer::*OP_Table_8XY_[15])() { &Lexer::OP_8XY0, &Lexer::OP_8XY1, &Lexer::OP_8XY2, &Lexer::OP_8XY3, &Lexer::OP_8XY4, &Lexer::OP_8XY5, &Lexer::OP_8XY6, &Lexer::OP_8XY7, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_8XYE };
	Ast* OP_8XY_();

	Ast* OP_9XY0(); //"9XY0", "If V[X] != V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
	Ast* OP_ANNN(); //"ANNN", "VI = NNN", { 0x0FFF }); }
	Ast* OP_BNNN(); //"BNNN", "PC = NNN + V0", { 0x0FFF }); }
	Ast* OP_CXNN(); //"CXNN", "V[X] = RAND(range:00-FF) & NN", { 0x0F00, 0x00FF }); }
	Ast* OP_DXYN(); //"DXYN", "Draw to screen", { 0x0F00, 0x00F0, 0x000F }); }

	Ast* OP_EX__(); //"EXA1", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 }); ("EX9E", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });

	Ast* OP_FX0_(); //"FX07", "V[X] = DELAY_TIMER", { 0x0F00 }); "FX0A", "Wait For Keypress, V[X] = KEY_VALUE", { 0x0F00 });

	Ast* OP_FX15(); //"FX15", "DELAY_TIMER = V[X]", { 0x0F00 }); }
	Ast* OP_FX18(); //"FX18", "SOUND_TIMER = V[X]", { 0x0F00 }); }
	Ast* OP_FX1E(); //"FX1E", "VI += V[X] w/ VF set in overflow", { 0x0F00 }); }

	Ast*(Lexer::*OP_Table_FX1_[15])() { &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_FX15, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_FX18, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_NULL, &Lexer::OP_FX1E };
	Ast* OP_FX1_();

	Ast* OP_FX29(); //"FX29", "VI = FONT @ V[X]", { 0x0F00 }); }
	Ast* OP_FX33(); //"FX33", "VI = BCD(V[X])", { 0x0F00 }); }
	Ast* OP_FX55(); //"FX55", "Put V0-[X] into MEMORY @ VI", { 0x0F00 }); }
	Ast* OP_FX65(); //"FX65", "Put MEMORY @ VI into V0-[X]", { 0x0F00 }); }

	Ast* (Lexer::*OP_Table_FX__[7])() { &Lexer::OP_FX0_, &Lexer::OP_FX1_, &Lexer::OP_FX29, &Lexer::OP_FX33, &Lexer::OP_NULL, &Lexer::OP_FX55, &Lexer::OP_FX65 };
	Ast* OP_FX__();

	Ast* (Lexer::*OP_Table_F000[16])() { &Lexer::OP_0___, &Lexer::OP_1NNN, &Lexer::OP_2NNN, &Lexer::OP_3XNN, &Lexer::OP_4XNN, &Lexer::OP_5XY0, &Lexer::OP_6XNN, &Lexer::OP_7XNN, &Lexer::OP_8XY_, &Lexer::OP_9XY0, &Lexer::OP_ANNN, &Lexer::OP_BNNN, &Lexer::OP_CXNN, &Lexer::OP_DXYN, &Lexer::OP_EX__, &Lexer::OP_FX__ };

public:

	Ast* LexInput(); 

	unsigned short word;
	unsigned char* program;
	unsigned int programLength;

	Lexer(unsigned char p[], unsigned int l);
	~Lexer();
};
#endif

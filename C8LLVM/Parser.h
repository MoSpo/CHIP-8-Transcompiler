#include "Lib.h"
#include "Ast.h"
#ifndef PARSER_H
#define PARSER_H
class Parser {

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

	Ast*(Parser::*OP_Table_8XY_[15])() { &Parser::OP_8XY0, &Parser::OP_8XY1, &Parser::OP_8XY2, &Parser::OP_8XY3, &Parser::OP_8XY4, &Parser::OP_8XY5, &Parser::OP_8XY6, &Parser::OP_8XY7, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_8XYE };
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

	Ast*(Parser::*OP_Table_FX1_[15])() { &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_FX15, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_FX18, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_NULL, &Parser::OP_FX1E };
	Ast* OP_FX1_();

	Ast* OP_FX29(); //"FX29", "VI = FONT @ V[X]", { 0x0F00 }); }
	Ast* OP_FX33(); //"FX33", "VI = BCD(V[X])", { 0x0F00 }); }
	Ast* OP_FX55(); //"FX55", "Put V0-[X] into MEMORY @ VI", { 0x0F00 }); }
	Ast* OP_FX65(); //"FX65", "Put MEMORY @ VI into V0-[X]", { 0x0F00 }); }

	Ast* (Parser::*OP_Table_FX__[7])() { &Parser::OP_FX0_, &Parser::OP_FX1_, &Parser::OP_FX29, &Parser::OP_FX33, &Parser::OP_NULL, &Parser::OP_FX55, &Parser::OP_FX65 };
	Ast* OP_FX__();

	Ast* (Parser::*OP_Table_F000[16])() { &Parser::OP_0___, &Parser::OP_1NNN, &Parser::OP_2NNN, &Parser::OP_3XNN, &Parser::OP_4XNN, &Parser::OP_5XY0, &Parser::OP_6XNN, &Parser::OP_7XNN, &Parser::OP_8XY_, &Parser::OP_9XY0, &Parser::OP_ANNN, &Parser::OP_BNNN, &Parser::OP_CXNN, &Parser::OP_DXYN, &Parser::OP_EX__, &Parser::OP_FX__ };

	std::vector<unsigned short> BR_NULL(unsigned short Index);
	std::vector<unsigned short> BR_00EE(unsigned short Index); //"00EE", "Return from subroutine", {});
	std::vector<unsigned short> BR_1NNN(unsigned short Index); //"1NNN", "PC = NNN", { 0x0FFF }); }
	std::vector<unsigned short> BR_2NNN(unsigned short Index); //"2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
	std::vector<unsigned short> BR_IF(unsigned short Index); //If [...], skip next instruction"
	std::vector<unsigned short> BR_BNNN(unsigned short Index); //"BNNN", "PC = NNN + V0", { 0x0FFF }); }

	std::vector<unsigned short> (Parser::*BR_Table_F000[16])(unsigned short Index) { &Parser::BR_00EE, &Parser::BR_1NNN, &Parser::BR_2NNN, &Parser::BR_IF, &Parser::BR_IF, &Parser::BR_IF, &Parser::BR_NULL, &Parser::BR_NULL, &Parser::BR_NULL, &Parser::BR_IF, &Parser::BR_NULL, &Parser::BR_BNNN, &Parser::BR_NULL, &Parser::BR_NULL, &Parser::BR_IF, &Parser::BR_NULL };

	unsigned short MemoryToBinaryAddress(unsigned short memoryAdr);

	unsigned short FindDataPartition();
	std::map<unsigned short, BasicBlock*> CreateBlocks(unsigned short dataPartition);
	std::map<unsigned short, BasicBlock*> LinkBlocks(std::map<unsigned short, BasicBlock*> blocksWithEntryAddresses);
	std::vector<BasicBlock*> FillBlocks(std::map<unsigned short, BasicBlock*> emptyBlocks, unsigned short dataPartition);

	std::map<unsigned short, unsigned short>addressPassName; //to check if something has been visited and if so by which pass to tell if a function, if so link to the function
	std::map<unsigned short, Ast*>addressLookup;
	std::vector<unsigned short> callStack;

	unsigned short word;
	unsigned char* program;
	unsigned int programLength;

public:

	std::vector<BasicBlock*> ParseInput();
	Ast* SimpleParseInput();

	Parser(unsigned char p[], unsigned int l);
	~Parser();
};
#endif

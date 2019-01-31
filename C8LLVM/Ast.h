#include "lib.h"
#ifndef AST_H
#define AST_H
class Ast {
public:
	unsigned int* operands;
	unsigned char operandsAmount;
	unsigned short instructionID;
	std::string stringID;
	//memeory location
	Ast* next;

	Ast(unsigned short instructionID, std::string stringID, unsigned int operands[], unsigned char opsAmt);
	~Ast();
};
#endif
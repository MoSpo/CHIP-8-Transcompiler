#include "Lib.h"
#ifndef AST_H
#define AST_H
class Ast {
public:
	unsigned int operands[3]{0,0,0};
	unsigned char operandsAmount;
	unsigned short instructionID;
	std::string stringID;
	//memeory location
	Ast* next;

	Ast(unsigned short iID, std::string sID, std::vector<unsigned int> operandVector, unsigned char opsAmt);
	~Ast();
};
#endif

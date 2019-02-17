#include "Ast.h"

Ast::Ast(unsigned short iID, std::string sID, std::vector<unsigned int> operandVector, unsigned char opsAmt) {
	instructionID = iID;
	stringID = sID;
	for (int i = 0; i < opsAmt; i++) {
		operands[i] = operandVector[i];
	}
	operandsAmount = opsAmt;
}


Ast::~Ast() {
	delete(nextInstruction);
	//delete(operands);
}

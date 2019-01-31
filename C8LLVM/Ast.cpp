#include "Ast.h"

Ast::Ast(unsigned short iID, std::string sID, unsigned int ops[], unsigned char opsAmt) {
	instructionID = iID;
	stringID = sID;
	operands = ops;
	operandsAmount = opsAmt;
}


Ast::~Ast() {
	delete(next);
	delete(operands);
}

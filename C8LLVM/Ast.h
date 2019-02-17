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
	Ast* nextInstruction;
	Ast* functionLink; // link to function header

	Ast(unsigned short iID, std::string sID, std::vector<unsigned int> operandVector, unsigned char opsAmt);
	~Ast();
};

class BasicBlock {
public:
	bool isFunctionBlock;
	unsigned int entryCount;
	unsigned int exitCount;
	unsigned short blockID; //the address of the first instruction in the block
	std::vector<unsigned short> entryAddresses;
	std::vector<BasicBlock*> entryBlocks;
	std::vector<BasicBlock*> exitBlocks;
	Ast* code;
	void MergeIntoBlock(BasicBlock* blockToMerge); //simplify 2 blocks
	BasicBlock();
	~BasicBlock();
};
#endif
#include "Lib.h"
#ifndef AST_H
#define AST_H
class Ast {
public:
	unsigned int operands[3]{0,0,0};
	unsigned char operandsAmount;
	unsigned short instructionID;
	std::string stringID;
	Ast* nextInstruction;

	Ast(unsigned short iID, std::string sID, std::vector<unsigned int> operandVector, unsigned char opsAmt);
	~Ast();
};

class BasicBlock {
public:
	bool isFunctionBlock;
	bool hasExplicitBranch;
	unsigned short visitedByFunctionID;
	unsigned short blockID; //the address of the first instruction in the block
	//std::vector<unsigned short> entryAddresses;
	//std::vector<BasicBlock*> entryBlockLinks;
	std::vector<BasicBlock*> functionBlockLinks;
	std::vector<BasicBlock*> exitBlockLinks;
	Ast* code;
	void MergeIntoBlock(BasicBlock* blockToMerge); //simplify 2 blocks
	BasicBlock();
	~BasicBlock();
};
#endif
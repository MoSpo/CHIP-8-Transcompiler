#include "Lib.h"
#include "Ast.h"
#ifndef TRACKER_H
#define TRACKER_H

class Tracker {
private:


public:

	Ast* treeEntry;
	Ast* currentNode;

	llvm::LLVMContext context;
	llvm::IRBuilder<> builder{ context };
	llvm::Module* mainModule;

	llvm::GlobalVariable* rX[16];

	llvm::GlobalVariable* rI;

	//std::map<std::string, llvm::Value> labeledData;
	//std::map<std::string, llvm::BasicBlock> labeledBlocks;

	llvm::Function* putchar;
	llvm::Function* main;

	llvm::BasicBlock* currentBlock;

	//errorlist

	void InitialiseRegisters();
    llvm::GlobalVariable* BuildRegister(llvm::Type *const &size, std::string name);
	Tracker(Ast* e);
	~Tracker();
};

#endif
#include "Generator.h"
#include "Flags.h"
#include <initializer_list>
#include <sstream>
#include <iomanip>

void Generator::OP_NULL(){}; //TODO: only load and store variables when needed, i.e. load when used and store all loaded for a function call

void Generator::OP_0___(){

	if (DEBUG && (opID & 0x00F0) == 0x00B0) { //OP_00BX
		auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
		auto zExtX32 = builder.CreateZExt(valX, builder.getInt32Ty());
		std::vector<llvm::Value *> args{ 1, zExtX32 };
		builder.CreateCall(putchar, args);
	} else if ((opID & 0x000F) == 0x0000) { //OP_00E0

	} else { //OP_00EE
		builder.CreateRet(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(32, 0x00)));
	}
};



void Generator::OP_1NNN(){}; //"1NNN", "PC = NNN", { 0x0FFF }); }
void Generator::OP_2NNN(){}; //"2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
void Generator::OP_3XNN(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto nn = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, currentNode->operands[1]));
	
	auto eq = builder.CreateICmpEQ(valX, nn); //check if the 2 values are the same
	
	builder.CreateCondBr(eq, GetLLVMBlock(currentBlock->exitBlockLinks[1]), GetLLVMBlock(currentBlock->exitBlockLinks[0]));
};

void Generator::OP_4XNN(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto nn = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, currentNode->operands[1]));

	auto eq = builder.CreateICmpEQ(valX, nn); //check if the 2 values are the same

	builder.CreateCondBr(eq, GetLLVMBlock(currentBlock->exitBlockLinks[0]), GetLLVMBlock(currentBlock->exitBlockLinks[1]));
};

void Generator::OP_5XY0(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);

	auto eq = builder.CreateICmpEQ(valX, valY); //check if the 2 values are the same

	builder.CreateCondBr(eq, GetLLVMBlock(currentBlock->exitBlockLinks[1]), GetLLVMBlock(currentBlock->exitBlockLinks[0]));
};

void Generator::OP_6XNN(){
	auto nn = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, currentNode->operands[1]));
	builder.CreateStore(nn, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_7XNN(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto nn = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, currentNode->operands[1]));

	auto addXnNN = builder.CreateAdd(valX, nn); //NOTE: What happens in overflow?

	builder.CreateStore(addXnNN, rX[currentNode->operands[0]/*X*/]);
};



void Generator::OP_8XY0(){
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);
	builder.CreateStore(valY, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY1(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);
	auto valOr = builder.CreateOr(valX, valY);
	builder.CreateStore(valOr, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY2(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);
	auto valOr = builder.CreateAnd(valX, valY);
	builder.CreateStore(valOr, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY3(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);
	auto valOr = builder.CreateXor(valX, valY);
	builder.CreateStore(valOr, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY4(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto zExtX16 = builder.CreateZExt(valX, builder.getInt16Ty());

	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);
	auto zExtY16 = builder.CreateZExt(valY, builder.getInt16Ty());

	auto addXnY16 = builder.CreateAdd(zExtX16, zExtY16); //add zero extended registers together

	auto byte = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(16, 255));
	auto oflw = builder.CreateICmpUGT(addXnY16, byte); //check if 16bit add is over 0xFF
	auto zExtOflw = builder.CreateZExt(oflw, builder.getInt8Ty());
	builder.CreateStore(zExtOflw, rX[15/*F*/]); //convert i1 bool to i8 and store it as overflow flag
	auto addXnY = builder.CreateTrunc(addXnY16, builder.getInt8Ty());
	builder.CreateStore(addXnY, rX[currentNode->operands[0]/*X*/]); //truncate and store
};

void Generator::OP_8XY5(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);

	auto subXnY = builder.CreateAdd(valX, valY);

	auto uflw = builder.CreateICmpUGT(valX, valY); //check if X > Y
	auto zExtUflw = builder.CreateZExt(uflw, builder.getInt8Ty());
	builder.CreateStore(zExtUflw, rX[15/*F*/]); //convert i1 bool to i8 and store it as underflow flag
	builder.CreateStore(subXnY, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY6(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto bit = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 0x01));

	//NOTE: On OG interpreter, the value of VY is shifted, not VX, and the result is stored into VX

	auto andX = builder.CreateAnd(valX, bit);
	builder.CreateStore(andX, rX[15/*F*/]);

	auto lShrX = builder.CreateLShr(valX, bit); //NOTE: may need to prove this will never poison?
	builder.CreateStore(lShrX, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY7(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);

	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);

	auto subYnX = builder.CreateAdd(valY, valX);

	auto uflw = builder.CreateICmpUGT(valY, valX); //check if Y > X
	auto zExtUflw = builder.CreateZExt(uflw, builder.getInt8Ty());
	builder.CreateStore(zExtUflw, rX[15/*F*/]); //convert i1 bool to i8 and store it as underflow flag
	builder.CreateStore(subYnX, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XYE(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto hiBit = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 7));
	auto loBit = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 1));

	//NOTE: On OG interpreter, the value of VY is shifted, not VX, and the result is stored into VX

	auto sigX = builder.CreateLShr(valX, hiBit);
	builder.CreateStore(sigX, rX[15/*F*/]);

	auto sftX = builder.CreateShl(valX, loBit);
	builder.CreateStore(sftX, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY_(){ { (this->*OP_Table_8XY_[opID & 0x000F])(); } };

void Generator::OP_9XY0(){}; //"9XY0", "If V[X] != V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
void Generator::OP_ANNN(){}; //"ANNN", "VI = NNN", { 0x0FFF }); }
void Generator::OP_BNNN(){}; //"BNNN", "PC = NNN + V0", { 0x0FFF }); }
void Generator::OP_CXNN(){}; //"CXNN", "V[X] = RAND(range:00-FF) & NN", { 0x0F00, 0x00FF }); }
void Generator::OP_DXYN(){}; //"DXYN", "Draw to screen", { 0x0F00, 0x00F0, 0x000F }); }

void Generator::OP_EX__(){}; // "EXA1", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 }); ("EX9E", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });

void Generator::OP_FX0_(){}; // "FX07", "V[X] = DELAY_TIMER", { 0x0F00 }); "FX0A", "Wait For Keypress, V[X] = KEY_VALUE", { 0x0F00 });

void Generator::OP_FX15(){}; //"FX15", "DELAY_TIMER = V[X]", { 0x0F00 }); }
void Generator::OP_FX18(){}; //"FX18", "SOUND_TIMER = V[X]", { 0x0F00 }); }
void Generator::OP_FX1E(){}; //"FX1E", "VI += V[X] w/ VF set in overflow", { 0x0F00 }); }

void Generator::OP_FX1_(){ (this->*OP_Table_FX1_[opID & 0x000F])(); };

void Generator::OP_FX29(){}; //"FX29", "VI = FONT @ V[X]", { 0x0F00 }); }
void Generator::OP_FX33(){}; //"FX33", "VI = BCD(V[X])", { 0x0F00 }); }
void Generator::OP_FX55(){}; //"FX55", "Put V0-[X] into MEMORY @ VI", { 0x0F00 }); }
void Generator::OP_FX65(){}; //"FX65", "Put MEMORY @ VI into V0-[X]", { 0x0F00 }); }

void Generator::OP_FX__(){ (this->*OP_Table_FX__[(opID & 0x00F0) >> 4])(); };

void Generator::SimpleGenerate(){
	while (currentNode->nextInstruction) {
		opID = currentNode->instructionID;
		(this->*OP_Table_F000[(opID & 0xF000) >> 12])();
		currentNode = currentNode->nextInstruction;
	}
	opID = currentNode->instructionID;
	(this->*OP_Table_F000[(opID & 0xF000) >> 12])();

	builder.CreateRet(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(32, 0x00)));

	mainModule->print(llvm::outs(), nullptr);
};

void Generator::Generate() {
	functionIndex = 1;
	for (BasicBlock* block : entryBlocks) {
		
		GenerateBlocks(std::vector<BasicBlock*>{block});
		blockMap.clear();
		functionIndex++;
	}
	mainModule->print(llvm::outs(), nullptr);
};

void Generator::GenerateBlocks(std::vector<BasicBlock*> blocksToVisit) { //loop properly through blocks here
	if (blocksToVisit.size() == 0) return; //no more blocks to do

	BasicBlock* blockToGenerate = blocksToVisit.back();
	currentBlock = blockToGenerate;
	blocksToVisit.pop_back();
	if (blockToGenerate->visitedByFunctionID != functionIndex) { //if not already visited
		builder.SetInsertPoint(GetLLVMBlock(blockToGenerate));

		currentNode = blockToGenerate->code;
		while (currentNode->nextInstruction) {
			opID = currentNode->instructionID;
			(this->*OP_Table_F000[(opID & 0xF000) >> 12])();
			currentNode = currentNode->nextInstruction;
		}
		opID = currentNode->instructionID; //for final instruction
		(this->*OP_Table_F000[(opID & 0xF000) >> 12])();

		if (!blockToGenerate->hasExplicitBranch) { //add branch or return instructions to all implicit branch links
			if (blockToGenerate->exitBlockLinks.size() > 0) {
				builder.CreateBr(GetLLVMBlock(blockToGenerate->exitBlockLinks[0]));
			} else {
				builder.CreateRet(llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(32, 0x00)));
			}
		}
		blockToGenerate->visitedByFunctionID = functionIndex; //set visited
		blocksToVisit.insert(blocksToVisit.end(), blockToGenerate->exitBlockLinks.begin(), blockToGenerate->exitBlockLinks.end()); //append branched to blocks to queue
	}
	GenerateBlocks(blocksToVisit);
};

Generator::Generator(std::vector<BasicBlock*> blocks) {
	mainModule = new llvm::Module("program", context);
	//basicBlocks = blocks;
	InitialiseRegisters();

	if (DEBUG) { //Add putchar
		std::vector<llvm::Type *> args{ 1, llvm::Type::getInt32Ty(context) };
		llvm::FunctionType *putCharFt = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), llvm::Type::getInt32Ty(context), false);
		llvm::Function *putCharF = llvm::Function::Create(putCharFt, llvm::Function::ExternalLinkage, "putchar", mainModule);
		putchar = putCharF;
	}

	llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);

	for (BasicBlock* block : blocks) {
		if (block->isFunctionBlock) { //find function blocks
			entryBlocks.push_back(block);
			llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "F_" + std::to_string(block->blockID), mainModule);
			functions.push_back(f);
			functionMap[block] = f;
			if (block->blockID = 0) { //setup entry block if found
				blockMap[block] = llvm::BasicBlock::Create(context, std::to_string(block->blockID), f);
			}
		}
	}
}

llvm::BasicBlock* Generator::GetLLVMBlock(BasicBlock* block) {
	if (blockMap.find(block) == blockMap.end()) { //if llvmbasicblock not made
		llvm::BasicBlock* b = llvm::BasicBlock::Create(context, std::to_string(block->blockID), functions[functionIndex]);
		blockMap[block] = b;
		return b;
	} else {
		return blockMap[block];
	}
}

void Generator::InitialiseRegisters() {
	rX[0] = BuildRegister(builder.getInt8Ty(), "r0");
	rX[1] = BuildRegister(builder.getInt8Ty(), "r1");
	rX[2] = BuildRegister(builder.getInt8Ty(), "r2");
	rX[3] = BuildRegister(builder.getInt8Ty(), "r3");
	rX[4] = BuildRegister(builder.getInt8Ty(), "r4");
	rX[5] = BuildRegister(builder.getInt8Ty(), "r5");
	rX[6] = BuildRegister(builder.getInt8Ty(), "r6");
	rX[7] = BuildRegister(builder.getInt8Ty(), "r7");
	rX[8] = BuildRegister(builder.getInt8Ty(), "r8");
	rX[9] = BuildRegister(builder.getInt8Ty(), "r9");
	rX[10] = BuildRegister(builder.getInt8Ty(), "rA");
	rX[11] = BuildRegister(builder.getInt8Ty(), "rB");
	rX[12] = BuildRegister(builder.getInt8Ty(), "rC");
	rX[13] = BuildRegister(builder.getInt8Ty(), "rD");
	rX[14] = BuildRegister(builder.getInt8Ty(), "rE");
	rX[15] = BuildRegister(builder.getInt8Ty(), "rF");

	//rI = BuildRegister(builder.getInt8Ty(), "rI");
}

llvm::GlobalVariable* Generator::BuildRegister(llvm::Type *const &size, std::string name) {

	mainModule->getOrInsertGlobal(name, size);
	llvm::GlobalVariable *gVar = mainModule->getNamedGlobal(name);
	gVar->setLinkage(llvm::GlobalValue::CommonLinkage);
	gVar->setAlignment(1);

	auto val = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 0));
	gVar->setInitializer(val);
	return gVar;
}

Generator::~Generator() {
	delete(mainModule);
	for (int i = 0; i < 16; i++) {
		//delete(rX[i]); NOTE: can only delete variables allocated by new
}

#include "Generator.h"
#include "Flags.h"
#include <initializer_list>
#include <sstream>
#include <iomanip>

void Generator::OP_NULL(){}; //TODO: only load and store variables when needed, i.e. load when used and store all loaded for a function call

void Generator::OP_0___(){
	if (DEBUG && (opID & 0x00F0) == 0x00B0) { //OP_00BX
		auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
		auto zExtX32 = t->builder.CreateZExt(valX, t->builder.getInt32Ty());
		std::vector<llvm::Value *> args{ 1, zExtX32 };
		t->builder.CreateCall(t->putchar, args);
	}
};

void Generator::OP_1NNN(){}; //"1NNN", "PC = NNN", { 0x0FFF }); }
void Generator::OP_2NNN(){}; //"2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
void Generator::OP_3XNN(){}; //"3XNN", "If V[X] == NNN, skip next instruction", { 0x0F00, 0x00FF }); }
void Generator::OP_4XNN(){}; //"4XNN", "If V[X] != NNN, skip next instruction", { 0x0F00, 0x00FF }); }
void Generator::OP_5XY0(){}; //"5XY0", "If V[X] == V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
void Generator::OP_6XNN(){
	auto nn = llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(8, t->currentNode->operands[1]));
	t->builder.CreateStore(nn, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_7XNN(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	auto nn = llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(8, t->currentNode->operands[1]));

	auto addXnNN = t->builder.CreateAdd(valX, nn); //NOTE: What happens in overflow?

	t->builder.CreateStore(addXnNN, t->rX[t->currentNode->operands[0]/*X*/]);
};

void Generator::OP_8XY0(){
	auto valY = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[1]/*Y*/]);
	t->builder.CreateStore(valY, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_8XY1(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	auto valY = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[1]/*Y*/]);
	auto valOr = t->builder.CreateOr(valX, valY);
	t->builder.CreateStore(valOr, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_8XY2(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	auto valY = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[1]/*Y*/]);
	auto valOr = t->builder.CreateAnd(valX, valY);
	t->builder.CreateStore(valOr, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_8XY3(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	auto valY = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[1]/*Y*/]);
	auto valOr = t->builder.CreateXor(valX, valY);
	t->builder.CreateStore(valOr, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_8XY4(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	auto zExtX16 = t->builder.CreateZExt(valX, t->builder.getInt16Ty());

	auto valY = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[1]/*Y*/]);
	auto zExtY16 = t->builder.CreateZExt(valY, t->builder.getInt16Ty());

	auto addXnY16 = t->builder.CreateAdd(zExtX16, zExtY16); //add zero extended registers together

	auto byte = llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(16, 255));
	auto oflw = t->builder.CreateICmpUGT(addXnY16, byte); //check if 16bit add is over 0xFF
	auto zExtOflw = t->builder.CreateZExt(oflw, t->builder.getInt8Ty());
	t->builder.CreateStore(zExtOflw, t->rX[15/*F*/]); //convert i1 bool to i8 and store it as overflow flag
	auto addXnY = t->builder.CreateTrunc(addXnY16, t->builder.getInt8Ty());
	t->builder.CreateStore(addXnY, t->rX[t->currentNode->operands[0]/*X*/]); //truncate and store
};
void Generator::OP_8XY5(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	
	auto valY = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[1]/*Y*/]);

	auto subXnY = t->builder.CreateAdd(valX, valY);

	auto uflw = t->builder.CreateICmpUGT(valX, valY); //check if X > Y
	auto zExtUflw = t->builder.CreateZExt(uflw, t->builder.getInt8Ty());
	t->builder.CreateStore(zExtUflw, t->rX[15/*F*/]); //convert i1 bool to i8 and store it as underflow flag
	t->builder.CreateStore(subXnY, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_8XY6(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	auto bit = llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(8, 0x01));

	//NOTE: On OG interpreter, the value of VY is shifted, not VX, and the result is stored into VX

	auto andX = t->builder.CreateAnd(valX, bit);
	t->builder.CreateStore(andX, t->rX[15/*F*/]);

	auto lShrX = t->builder.CreateLShr(valX, bit); //NOTE: may need to prove this will never poison?
	t->builder.CreateStore(lShrX, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_8XY7(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);

	auto valY = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[1]/*Y*/]);

	auto subYnX = t->builder.CreateAdd(valY, valX);

	auto uflw = t->builder.CreateICmpUGT(valY, valX); //check if Y > X
	auto zExtUflw = t->builder.CreateZExt(uflw, t->builder.getInt8Ty());
	t->builder.CreateStore(zExtUflw, t->rX[15/*F*/]); //convert i1 bool to i8 and store it as underflow flag
	t->builder.CreateStore(subYnX, t->rX[t->currentNode->operands[0]/*X*/]);
};
void Generator::OP_8XYE(){
	auto valX = t->builder.CreateLoad(t->builder.getInt8Ty(), t->rX[t->currentNode->operands[0]/*X*/]);
	auto hiBit = llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(8, 7));
	auto loBit = llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(8, 1));

	//NOTE: On OG interpreter, the value of VY is shifted, not VX, and the result is stored into VX

	auto sigX = t->builder.CreateLShr(valX, hiBit);
	t->builder.CreateStore(sigX, t->rX[15/*F*/]);

	auto sftX = t->builder.CreateShl(valX, loBit);
	t->builder.CreateStore(sftX, t->rX[t->currentNode->operands[0]/*X*/]);
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

void Generator::Generate(){
	while (t->currentNode->nextInstruction) {
		opID = t->currentNode->instructionID;
		(this->*OP_Table_F000[(opID & 0xF000) >> 12])();
		t->currentNode = t->currentNode->nextInstruction;
	}
	opID = t->currentNode->instructionID;
	(this->*OP_Table_F000[(opID & 0xF000) >> 12])();

	t->builder.CreateRet(llvm::ConstantInt::get(t->mainModule->getContext(), llvm::APInt(32, 0x00)));

	t->mainModule->print(llvm::outs(), nullptr);
};

Generator::Generator(Ast* e) {
	t = new Tracker{ e };
};
Generator::~Generator() {
	delete(t); //TODO: Remove Tracker and merge
};

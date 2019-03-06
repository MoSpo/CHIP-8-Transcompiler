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

#pragma region finishedstuff

void Generator::OP_1NNN(){
	builder.CreateBr(GetLLVMBlock(currentBlock->exitBlockLinks[0]));
};

void Generator::OP_2NNN(){
	builder.CreateCall(functionMap[currentBlock->functionBlockLinks[functionCallIndex]]);
	functionCallIndex++;
};

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

void Generator::OP_9XY0(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]/*Y*/]);

	auto eq = builder.CreateICmpEQ(valX, valY); //check if the 2 values are the same

	builder.CreateCondBr(eq, GetLLVMBlock(currentBlock->exitBlockLinks[0]), GetLLVMBlock(currentBlock->exitBlockLinks[1]));
};

#pragma endregion

void Generator::OP_ANNN(){
	auto nnn = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(16, currentNode->operands[1] - 512 - codeLength)); //so that we can use this to read propery into our array - we also don't need the exact value of rI, so can do this
	builder.CreateStore(nnn, rI);

	//auto flagSet = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(1, 0));
	//builder.CreateStore(useFont, flagSet); //uses main memory
};

void Generator::OP_BNNN(){}; //"BNNN", Indirect Jump TODO: much later

void Generator::OP_CXNN(){
	auto rand = builder.CreateCall(getRand);
	auto andRand = builder.CreateAnd(rand, llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, currentNode->operands[1])));
	builder.CreateStore(andRand, rX[currentNode->operands[0]/*X*/]);
};

void Generator::OP_DXYN(){
	/*entry:
	 br label %outerloop
  
	outerloop: ; preds = %afterloop, %entry
	  %var = phi i8 [ nextj, %afterloop ], [  0, %entry ]
	  br label %innerloop

	innerloop: ; preds = %outerloop, %innerloop

	  %j = phi i8 [ var, %outerloop ]
	  %i = phi i8 [ nexti, %innerloop ], [  0, %outerloop ]

	  ; body
	  bit = ((MEMORY[REG_VI + j] & (128 >> i)) >> (7 - i));
	  x = (REG_V[(WORD & 0x0F00) >> 8] + i) % 64;
	  y = (REG_V[(WORD & 0x00F0) >> 4] + j) % 32;
	  %calltmp = call double @setpixel(x,y,bit)
	  R[F] = calltmp
  
	  ; increment
	  %nexti = fadd i8 %i, 1

	  ; termination test //i == 8
	  %cmpi = fcmpeq i8 %i, 8
	  br i1 %cmpi, label %innerloop, label %afterloop

	afterloop: ; preds = %innerloop
	  ; increment
	  %varj = phi i8 [ j, %innerloop ]
	  %nextj = fadd i8 %varj, 1

	  ; termination test //i == n
	  %cmpj = fcmp ult i8 %i, %n
	  br i1 %cmpj, label %outerloop, label %exit

	exit: ; preds = %afterloop
	*/
    
	llvm::BasicBlock* outerloop = llvm::BasicBlock::Create(context, std::to_string(currentBlock->blockID) + "_ol", functions[functionIndex - 1]);
	llvm::BasicBlock* innerloop = llvm::BasicBlock::Create(context, std::to_string(currentBlock->blockID) + "_il", functions[functionIndex - 1]);
	llvm::BasicBlock* afterloop = llvm::BasicBlock::Create(context, std::to_string(currentBlock->blockID) + "_al", functions[functionIndex - 1]);
	llvm::BasicBlock* exit = llvm::BasicBlock::Create(context, std::to_string(currentBlock->blockID) + "_b", functions[functionIndex - 1]);

	auto one = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 1));
	builder.CreateBr(outerloop);
	//outerloop
	builder.SetInsertPoint(outerloop);

	llvm::PHINode* vj_a = builder.CreatePHI(llvm::Type::getInt8Ty(mainModule->getContext()), 2);
	vj_a->addIncoming(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 0)), GetLLVMBlock(currentBlock));

	builder.CreateBr(innerloop);
	//innerloop
	builder.SetInsertPoint(innerloop);

	llvm::PHINode* j = builder.CreatePHI(llvm::Type::getInt8Ty(mainModule->getContext()), 2);
	j->addIncoming(vj_a, outerloop);
	j->addIncoming(j, innerloop);

	llvm::PHINode* i = builder.CreatePHI(llvm::Type::getInt8Ty(mainModule->getContext()), 2);
	i->addIncoming(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 0)), outerloop);

	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]]);
	auto valY = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[1]]);
	auto valXaddi = builder.CreateAdd(i, valX); //NOTE: Could overflow here
	auto valYaddj = builder.CreateAdd(j, valY);
	auto Ximod = builder.CreateURem(valXaddi, llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 64)));
	auto Yjmod = builder.CreateURem(valYaddj, llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 32)));

	//TODO: NEED TO ADD FONT CHECK HERE
	auto valI = builder.CreateLoad(builder.getInt16Ty(), rI);
    auto zExtj16 = builder.CreateZExt(j, builder.getInt16Ty());
	auto valIaddj = builder.CreateAdd(zExtj16, valI); //NOTE: Can't overflow here
    
    std::vector<llvm::Value *> gepVect;
    gepVect.push_back(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(16, 0)));
    gepVect.push_back(valIaddj);
	auto SpriteGEP = builder.CreateGEP(Memory, llvm::ArrayRef<llvm::Value *>(gepVect));

	auto lShr128 = builder.CreateLShr(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 128)), i);
	auto min7i = builder.CreateSub(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 7)), i);
    //SpriteGEP->getType()->print(llvm::outs());
    auto SprVal = builder.CreateLoad(builder.getInt8Ty(), SpriteGEP);
	auto Sprand128 = builder.CreateAnd(SprVal, lShr128);

	auto lShr7i = builder.CreateLShr(Sprand128, min7i);

	std::vector<llvm::Value *> args{Ximod, Yjmod, lShr7i};
	auto ret = builder.CreateCall(setPixel, args);
	builder.CreateStore(ret, rX[15]);

	auto nexti = builder.CreateAdd(i, one);
	i->addIncoming(nexti, innerloop);

	auto eqeight = builder.CreateICmpEQ(nexti, llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 8)));
	builder.CreateCondBr(eqeight, afterloop, innerloop);
	//afterloop
	builder.SetInsertPoint(afterloop);

	llvm::PHINode* vj_b = builder.CreatePHI(llvm::Type::getInt8Ty(mainModule->getContext()), 1);
	vj_b->addIncoming(j, innerloop);

	auto nextj = builder.CreateAdd(vj_b, one);
	vj_a->addIncoming(nextj, afterloop);

	auto eqn = builder.CreateICmpEQ(nextj, llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, currentNode->operands[2])));
	builder.CreateCondBr(eqn, exit, outerloop);
	//exit
	builder.SetInsertPoint(exit);
	blockMap[currentBlock] = exit;
}; //"DXYN", "Draw to screen", { 0x0F00, 0x00F0, 0x000F }); }

void Generator::OP_EX__(){}; // "EXA1", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 }); ("EX9E", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });

void Generator::OP_FX0_(){}; // "FX07", "V[X] = DELAY_TIMER", { 0x0F00 }); "FX0A", "Wait For Keypress, V[X] = KEY_VALUE", { 0x0F00 });

void Generator::OP_FX15(){}; //"FX15", "DELAY_TIMER = V[X]", { 0x0F00 }); }
void Generator::OP_FX18(){}; //"FX18", "SOUND_TIMER = V[X]", { 0x0F00 }); }
void Generator::OP_FX1E(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto zExtX32 = builder.CreateZExt(valX, builder.getInt32Ty());

	auto valI = builder.CreateLoad(builder.getInt16Ty(), rI);
	auto zExtI32 = builder.CreateZExt(valI, builder.getInt32Ty());

	auto addXnI32 = builder.CreateAdd(zExtX32, zExtI32); //add zero extended registers together

	auto byte = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(32, 65535));
	auto oflw = builder.CreateICmpUGT(addXnI32, byte); //check if is over 0xFFFF
	auto zExtOflw = builder.CreateZExt(oflw, builder.getInt8Ty());
	builder.CreateStore(zExtOflw, rX[15/*F*/]); //convert i1 bool to i8 and store it as overflow flag
	auto addXnI = builder.CreateTrunc(addXnI32, builder.getInt16Ty());
	builder.CreateStore(addXnI, rI); //truncate and store
};

void Generator::OP_FX1_(){ (this->*OP_Table_FX1_[opID & 0x000F])(); };


void Generator::OP_FX29(){
	auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	auto zExtX16 = builder.CreateZExt(valX, builder.getInt16Ty());

	auto amt = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(16, 5));
	auto mult = builder.CreateMul(zExtX16, amt);

	//auto font = builder.CreateGEP(Fonts, valX); //gets the pointer for the value inside the array(?)
	builder.CreateStore(mult, rI);

	//auto flagSet = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(1, 1));
	//builder.CreateStore(useFont, flagSet); //uses font memory
};
void Generator::OP_FX33(){

	//Assume useFont not set
	//auto valX = builder.CreateLoad(builder.getInt8Ty(), rX[currentNode->operands[0]/*X*/]);
	/*
	auto hndrd = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 100));
	auto ten = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 10));

	auto divTen = builder.CreateUDiv(valX, ten);

	auto top = builder.CreateUDiv(valX, hndrd);
	auto mid = builder.CreateURem(divTen, ten);
	auto bot = builder.CreateURem(valX, ten);

	//auto font = builder.CreateGEP(Fonts, valX); //gets the pointer for the value inside the array(?)
	*/
}; //"FX33", "VI = BCD(V[X])", { 0x0F00 }); }
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
		if (block->blockID == 0) { //adds the entry blocks, so we don't run into the problem of the entry blocks having predecessors! 
			llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", functionMap[block]);
			builder.SetInsertPoint(entry);
			builder.CreateCall(IOInit);
			builder.CreateBr(GetLLVMBlock(block));
		} else {
			llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, functionIndex + "_entry", functionMap[block]);
			builder.SetInsertPoint(entry);
			builder.CreateBr(GetLLVMBlock(block));
		}
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
		
		functionCallIndex = 0;
		currentNode = blockToGenerate->code;
		while (currentNode->nextInstruction) {
			builder.CreateCall(IOLoop); //LOOPS
			opID = currentNode->instructionID;
			(this->*OP_Table_F000[(opID & 0xF000) >> 12])();
			currentNode = currentNode->nextInstruction;
		}
		builder.CreateCall(IOLoop); //LOOPS
		opID = currentNode->instructionID; //for final instruction
		(this->*OP_Table_F000[(opID & 0xF000) >> 12])();

		if (!blockToGenerate->hasExplicitBranch) { //add branch or return instructions to all implicit branch links
			if (blockToGenerate->exitBlockLinks.size() > 0) {
				builder.CreateBr(GetLLVMBlock(blockToGenerate->exitBlockLinks[0]));
			} else {
				builder.CreateRet(llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(32, 0x00)));
			}
		}
		blockToGenerate->visitedByFunctionID = functionIndex; //set visited
		blocksToVisit.insert(blocksToVisit.end(), blockToGenerate->exitBlockLinks.begin(), blockToGenerate->exitBlockLinks.end()); //append branched to blocks to queue
	}
	GenerateBlocks(blocksToVisit);
};

Generator::Generator(std::vector<BasicBlock*> code, std::vector<unsigned char> data, unsigned short dataPartition) {
	mainModule = new llvm::Module("program", context);
	//basicBlocks = blocks;

	codeLength = dataPartition;
	InitialiseRegisters();
	InitialiseMemory(data);

	if (DEBUG) { //Add putchar
		llvm::FunctionType *putCharFt = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), llvm::Type::getInt32Ty(context), false);
		llvm::Function *putCharF = llvm::Function::Create(putCharFt, llvm::Function::ExternalLinkage, "putchar", mainModule);
		putchar = putCharF;
	}

	llvm::FunctionType *IOFt = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
	llvm::Function *IOInitF = llvm::Function::Create(IOFt, llvm::Function::ExternalLinkage, "IOInit", mainModule);
	IOInit = IOInitF;
	llvm::Function *IOLoopF = llvm::Function::Create(IOFt, llvm::Function::ExternalLinkage, "IOLoop", mainModule);
	IOLoop = IOLoopF;

	llvm::FunctionType *getRandFt = llvm::FunctionType::get(llvm::Type::getInt8Ty(context), false);
	llvm::Function *getRandF = llvm::Function::Create(getRandFt, llvm::Function::ExternalLinkage, "GetRand", mainModule);
	getRand = getRandF;

	std::vector<llvm::Type *> pixArgs{ llvm::Type::getInt8Ty(context), llvm::Type::getInt8Ty(context), llvm::Type::getInt8Ty(context) };
	llvm::FunctionType *setPixelFt = llvm::FunctionType::get(llvm::Type::getInt8Ty(context), pixArgs, false);
	llvm::Function *setPixelF = llvm::Function::Create(setPixelFt, llvm::Function::ExternalLinkage, "SetPixel", mainModule);
	setPixel = setPixelF;

	llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);

	for (BasicBlock* block : code) {
		if (block->isFunctionBlock) { //find function blocks
			entryBlocks.push_back(block);
            std::string name;
            if (block->blockID == 0) {
                name = "main";
            } else {
                name = "F_" + std::to_string(block->blockID);
            }
			llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, mainModule);
			functions.push_back(f);
			functionMap[block] = f;
			if (block->blockID == 0) { //setup entry block if found
				blockMap[block] = llvm::BasicBlock::Create(context, std::to_string(block->blockID), f);
			}
		}
	}
}

llvm::BasicBlock* Generator::GetLLVMBlock(BasicBlock* block) {
	if (blockMap.find(block) == blockMap.end()) { //if llvmbasicblock not made
		llvm::BasicBlock* b = llvm::BasicBlock::Create(context, std::to_string(block->blockID), functions[functionIndex-1]);
		blockMap[block] = b;
		return b;
	} else {
		return blockMap[block];
	}
}

unsigned short Generator::MemoryToBinaryAddress(unsigned short memoryAdr) {
	return (memoryAdr - 512) / 2;
}

void Generator::InitialiseRegisters() {
	auto ty = builder.getInt8Ty();

	rX[0] = BuildRegister(ty, 8, "r0");
	rX[1] = BuildRegister(ty, 8, "r1");
	rX[2] = BuildRegister(ty, 8, "r2");
	rX[3] = BuildRegister(ty, 8, "r3");
	rX[4] = BuildRegister(ty, 8, "r4");
	rX[5] = BuildRegister(ty, 8, "r5");
	rX[6] = BuildRegister(ty, 8, "r6");
	rX[7] = BuildRegister(ty, 8, "r7");
	rX[8] = BuildRegister(ty, 8, "r8");
	rX[9] = BuildRegister(ty, 8, "r9");
	rX[10] = BuildRegister(ty, 8, "rA");
	rX[11] = BuildRegister(ty, 8, "rB");
	rX[12] = BuildRegister(ty, 8, "rC");
	rX[13] = BuildRegister(ty, 8, "rD");
	rX[14] = BuildRegister(ty, 8, "rE");
	rX[15] = BuildRegister(ty, 8, "rF");

	rI = BuildRegister(builder.getInt16Ty(), 16, "rI");
}

void Generator::InitialiseMemory(std::vector<unsigned char> data) {
	mainModule->getOrInsertGlobal("Memory", llvm::ArrayType::get(builder.getInt8Ty(), data.size()));
	Memory = mainModule->getNamedGlobal("Memory");
	Memory->setLinkage(llvm::GlobalValue::PrivateLinkage);
	Memory->setAlignment(16);

	std::vector<llvm::Constant*> memValues;
	for(unsigned char d : data) {
		llvm::Constant* c = llvm::Constant::getIntegerValue(builder.getInt8Ty(), llvm::APInt(8, d));
		memValues.push_back(c);
	}
	Memory->setInitializer(llvm::ConstantArray::get(llvm::ArrayType::get(builder.getInt8Ty(), data.size()), memValues));

	unsigned char Fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	mainModule->getOrInsertGlobal("Fonts", llvm::ArrayType::get(builder.getInt8Ty(), 80));
	Fonts = mainModule->getNamedGlobal("Fonts");
	Fonts->setLinkage(llvm::GlobalValue::PrivateLinkage);
	Fonts->setAlignment(16);

	std::vector<llvm::Constant*> fontValues;
	for (unsigned char f : Fontset) {
		llvm::Constant* c = llvm::Constant::getIntegerValue(builder.getInt8Ty(), llvm::APInt(8, f));
		fontValues.push_back(c);
	}
	Fonts->setInitializer(llvm::ConstantArray::get(llvm::ArrayType::get(builder.getInt8Ty(), 80), fontValues));

	useFont = BuildRegister(builder.getInt1Ty(), 1, "useFont");
}

llvm::GlobalVariable* Generator::BuildRegister(llvm::Type *const &type, unsigned char size, std::string name) {

	mainModule->getOrInsertGlobal(name, type);
	llvm::GlobalVariable *gVar = mainModule->getNamedGlobal(name);
	gVar->setLinkage(llvm::GlobalValue::CommonLinkage);
	gVar->setAlignment(size/8);

	auto val = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(size, 0));
	gVar->setInitializer(val);
	return gVar;
}

Generator::~Generator() {
	delete(mainModule);
	for (int i = 0; i < 16; i++) {
		//delete(rX[i]); NOTE: can only delete variables allocated by new
    }
}

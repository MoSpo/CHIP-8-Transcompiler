#include "Tracker.h"
#include "Flags.h"

Tracker::Tracker(Ast* e) {
	treeEntry = e;
	currentNode = e;
	mainModule = new llvm::Module("toplevel", context);
	InitialiseRegisters();

	if (DEBUG) {
		std::vector<llvm::Type *> args{ 1, llvm::Type::getInt32Ty(context) };
		llvm::FunctionType *putCharFt = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), llvm::Type::getInt32Ty(context), false);
		llvm::Function *putCharF = llvm::Function::Create(putCharFt, llvm::Function::ExternalLinkage, "putchar", mainModule);
		putchar = putCharF;
	}

	llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
	llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", mainModule);
	main = f;
	currentBlock = llvm::BasicBlock::Create(context, "entry", f);
	builder.SetInsertPoint(currentBlock);
}

void Tracker::InitialiseRegisters() {
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

llvm::GlobalVariable* Tracker::BuildRegister(llvm::Type *const &size, std::string name) {

	mainModule->getOrInsertGlobal(name, size);
	llvm::GlobalVariable *gVar = mainModule->getNamedGlobal(name);
	gVar->setLinkage(llvm::GlobalValue::CommonLinkage);
	gVar->setAlignment(1);

	auto val = llvm::ConstantInt::get(mainModule->getContext(), llvm::APInt(8, 0));
	gVar->setInitializer(val);
	return gVar;
}

Tracker::~Tracker() {
	delete(treeEntry);
	delete(mainModule);
	for (int i = 0; i < 16; i++) {
		delete(rX[i]);
	}
	delete(rX);
	delete(rI);
	delete(putchar);
	delete(main);
	delete(currentBlock);
}

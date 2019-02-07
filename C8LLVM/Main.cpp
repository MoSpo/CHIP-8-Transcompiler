/*#include "Tracker.h"
#include "Generator.h"
#include "main.h"

int main(int argc, char**argv) {
	Tracker t{};

	//TheModule = makeLLVMModule();

	//verifyModule(*Mod, PrintMessageAction);

	//PassManager PM;
	//PM.add(createPrintModulePass(&outs()));
	//PM.run(*Mod);

	delete Mod;
	return 0;
}*/

#include "Lib.h"
#include "Lexer.h"
#include "Generator.h"
#include "Ast.h"
#include "Flags.h"
static unsigned char program[3696];
static unsigned int length;

bool DEBUG;

bool Error(const char msg[]) {
	std::cout << msg << std::endl;
	return false;
}

bool OpenInputFile(int argc, char *argv[]) {
	if (argc <= 1) return Error("No file");
	else {
		std::ifstream file;
		file.open(argv[argc - 1], std::ios::binary);
		if (!file) return Error("Unable to open file");

		int i = 0;
		length = 0;

		while (file) {
			file.read(reinterpret_cast<char*>(&program[i]), 1);
			file.read(reinterpret_cast<char*>(&program[i + 1]), 1);
			i += 2;
		}

		file.close();

		length = i;
		for (; i < 3696; i++) program[i] = 0;

	}
	return true;
}

bool ReadFlags(int argc, char *argv[]) {
	DEBUG = true;
	return true;
}

int main(int argc, char *argv[]) {
	if (OpenInputFile(argc, argv)) {
		if (!ReadFlags(argc, argv)) return -1;

		Lexer l{program, length};
		Ast* entry = l.LexInput();
		Generator g{entry};
		g.Generate();
		/*std::ofstream file;
		std::string str(argv[argc - 1]);
		file.open(str + "asm");
		int PC = 0;

		word = (program[PC] << 8) | (program[PC + 1]);
		file << OP_Lex();
		PC += 2;
		while (PC < length) {
			file << "\n";
			word = (program[PC] << 8) | (program[PC + 1]);
			file << OP_Lex();
			PC += 2;
		}

		file.close();*/

	}
	else return -1;
	return 0;
}

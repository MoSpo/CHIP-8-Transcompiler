#include "Lexer.h"
#include "Ast.h"
#include "Flags.h"

using namespace std;

Ast* Lexer::OP_Eval(const string& operationName, const string& operationDescription, const vector<unsigned int>& operandMasks) {

	unsigned char l = 0;
	vector<unsigned int> opStore{};
    for (unsigned short msk : operandMasks) {
        int i = 0;
        while (((msk >> i) & 0x1) == 0) {
            i += 4;
        }

		opStore.push_back((word & msk) >> i);
		l++;
    }

    //if (VERBOSE) { operationDescription; }

	Ast* a = new Ast{ word, operationName, opStore, l };
    return a;
}

Ast* Lexer::OP_NULL() {
    return NULL; //TODO
}

Ast* Lexer::OP_0___() {
	if(DEBUG && (word & 0x00F0) == 0x00B0) return OP_Eval("00BX", "print V[X] (Debug only)", { 0x000F });
	else if ((word & 0x000F) == 0x0000) return OP_Eval("00E0", "Clear the display", {});
	else return OP_Eval("00EE", "Return from subroutine", {});
}

Ast* Lexer::OP_1NNN() { return OP_Eval("1NNN", "PC = NNN", { 0x0FFF }); }
Ast* Lexer::OP_2NNN() { return OP_Eval("2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
Ast* Lexer::OP_3XNN() { return OP_Eval("3XNN", "If V[X] == NNN, skip next instruction", { 0x0F00, 0x00FF }); }
Ast* Lexer::OP_4XNN() { return OP_Eval("4XNN", "If V[X] != NNN, skip next instruction", { 0x0F00, 0x00FF }); }
Ast* Lexer::OP_5XY0() { return OP_Eval("5XY0", "If V[X] == V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_6XNN() { return OP_Eval("6XNN", "V[X] = NN", { 0x0F00, 0x00FF }); }
Ast* Lexer::OP_7XNN() { return OP_Eval("7XNN", "V[X] += NN", { 0x0F00, 0x00FF }); }


Ast* Lexer::OP_8XY0() { return OP_Eval("8XY0", "V[X] = V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XY1() { return OP_Eval("8XY1", "V[X] |= V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XY2() { return OP_Eval("8XY2", "V[X] &= V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XY3() { return OP_Eval("8XY3", "V[X] ^= V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XY4() { return OP_Eval("8XY4", "V[X] += V[Y] w/ VF set in overflow", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XY5() { return OP_Eval("8XY5", "V[X] -= V[Y] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XY6() { return OP_Eval("8XY6", "V[X] >>= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XY7() { return OP_Eval("8XY7", "V[X] = V[Y] - V[X] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_8XYE() { return OP_Eval("8XYE", "V[X] <<= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }

Ast* Lexer::OP_8XY_() { return (this->*OP_Table_8XY_[word & 0x000F])(); }

Ast* Lexer::OP_9XY0() { return OP_Eval("9XY0", "If V[X] != V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
Ast* Lexer::OP_ANNN() { return OP_Eval("ANNN", "VI = NNN", { 0x0FFF }); }
Ast* Lexer::OP_BNNN() { return OP_Eval("BNNN", "PC = NNN + V0", { 0x0FFF }); }
Ast* Lexer::OP_CXNN() { return OP_Eval("CXNN", "V[X] = RAND(range:00-FF) & NN", { 0x0F00, 0x00FF }); }
Ast* Lexer::OP_DXYN() { return OP_Eval("DXYN", "Draw to screen", { 0x0F00, 0x00F0, 0x000F }); }

Ast* Lexer::OP_EX__() {
	if ((word & 0x000F) == 0x0001) return OP_Eval("EXA1", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });
	else return OP_Eval("EX9E", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });
}

Ast* Lexer::OP_FX0_() {
    if ((word & 0x000F) == 0x0007) return OP_Eval("FX07", "V[X] = DELAY_TIMER", { 0x0F00 });
    else return OP_Eval("FX0A", "Wait For Keypress, V[X] = KEY_VALUE", { 0x0F00 });
}

Ast* Lexer::OP_FX15() { return OP_Eval("FX15", "DELAY_TIMER = V[X]", { 0x0F00 }); }
Ast* Lexer::OP_FX18() { return OP_Eval("FX18", "SOUND_TIMER = V[X]", { 0x0F00 }); }
Ast* Lexer::OP_FX1E() { return OP_Eval("FX1E", "VI += V[X] w/ VF set in overflow", { 0x0F00 }); }

Ast* Lexer::OP_FX1_() { return (this->*OP_Table_FX1_[word & 0x000F])(); } //FX15, FX18, FX1E

Ast* Lexer::OP_FX29() { return OP_Eval("FX29", "VI = FONT @ V[X]", { 0x0F00 }); }
Ast* Lexer::OP_FX33() { return OP_Eval("FX33", "VI = BCD(V[X])", { 0x0F00 }); }
Ast* Lexer::OP_FX55() { return OP_Eval("FX55", "Put V0-[X] into MEMORY @ VI", { 0x0F00 }); }
Ast* Lexer::OP_FX65() { return OP_Eval("FX65", "Put MEMORY @ VI into V0-[X]", { 0x0F00 }); }

Ast* Lexer::OP_FX__() { return (this->*OP_Table_FX__[(word & 0x00F0) >> 4])(); }


Ast* Lexer::LexInput() {
	unsigned int PC = 0;
	word = (program[PC] << 8) | (program[PC + 1]);
	Ast* Entry = (this->*OP_Table_F000[(word & 0xF000) >> 12])(); //Setup first node
	Ast* Current = Entry;
	PC += 2;
	while (PC < programLength) { //Loop for the rest of the program
		word = (program[PC] << 8) | (program[PC + 1]);
		Current->next = (this->*OP_Table_F000[(word & 0xF000) >> 12])();
		Current = Current->next;
		PC += 2;
	}
	return Entry;
}

Lexer::Lexer(unsigned char p[], unsigned int l) {
	program = p;
	programLength = l;
};

Lexer::~Lexer() {};

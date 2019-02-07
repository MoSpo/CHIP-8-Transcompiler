#include "Tracker.h"
#include "Ast.h"
#ifndef GENERATOR_H
#define GENERATOR_H
class Generator {

private:
	Tracker* t; //TODO: Remove Tracker and merge

	void OP_NULL();

	void OP_0___(); //"00BX" print V[X] (Debug only), {0x000F}); "00E0", "Clear the display", {}); "00EE", "Return from subroutine", {});

	void OP_1NNN(); //"1NNN", "PC = NNN", { 0x0FFF }); }
	void OP_2NNN(); //"2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
	void OP_3XNN(); //"3XNN", "If V[X] == NNN, skip next instruction", { 0x0F00, 0x00FF }); }
	void OP_4XNN(); //"4XNN", "If V[X] != NNN, skip next instruction", { 0x0F00, 0x00FF }); }
	void OP_5XY0(); //"5XY0", "If V[X] == V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
	void OP_6XNN(); //"6XNN", "V[X] = NN", { 0x0F00, 0x00FF }); }
	void OP_7XNN(); //"7XNN", "V[X] += NN", { 0x0F00, 0x00FF }); }

	void OP_8XY0(); //"8XY0", "V[X] = V[Y]", { 0x0F00, 0x00F0 }); }
	void OP_8XY1(); //"8XY1", "V[X] |= V[Y]", { 0x0F00, 0x00F0 }); }
	void OP_8XY2(); //"8XY2", "V[X] &= V[Y]", { 0x0F00, 0x00F0 }); }
	void OP_8XY3(); //"8XY3", "V[X] ^= V[Y]", { 0x0F00, 0x00F0 }); }
	void OP_8XY4(); //"8XY4", "V[X] += V[Y] w/ VF set in overflow", { 0x0F00, 0x00F0 }); }
	void OP_8XY5(); //"8XY5", "V[X] -= V[Y] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
	void OP_8XY6(); //"8XY6", "V[X] >>= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }
	void OP_8XY7(); //"8XY7", "V[X] = V[Y] - V[X] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
	void OP_8XYE(); //"8XYE", "V[X] <<= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }

	void(Generator::*OP_Table_8XY_[15])(){ &Generator::OP_8XY0, &Generator::OP_8XY1, &Generator::OP_8XY2, &Generator::OP_8XY3, &Generator::OP_8XY4, &Generator::OP_8XY5, &Generator::OP_8XY6, &Generator::OP_8XY7, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_8XYE };
	void OP_8XY_();

	void OP_9XY0(); //"9XY0", "If V[X] != V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
	void OP_ANNN(); //"ANNN", "VI = NNN", { 0x0FFF }); }
	void OP_BNNN(); //"BNNN", "PC = NNN + V0", { 0x0FFF }); }
	void OP_CXNN(); //"CXNN", "V[X] = RAND(range:00-FF) & NN", { 0x0F00, 0x00FF }); }
	void OP_DXYN(); //"DXYN", "Draw to screen", { 0x0F00, 0x00F0, 0x000F }); }

	void OP_EX__(); //"EXA1", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 }); ("EX9E", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });

	void OP_FX0_(); //"FX07", "V[X] = DELAY_TIMER", { 0x0F00 }); "FX0A", "Wait For Keypress, V[X] = KEY_VALUE", { 0x0F00 });

	void OP_FX15(); //"FX15", "DELAY_TIMER = V[X]", { 0x0F00 }); }
	void OP_FX18(); //"FX18", "SOUND_TIMER = V[X]", { 0x0F00 }); }
	void OP_FX1E(); //"FX1E", "VI += V[X] w/ VF set in overflow", { 0x0F00 }); }

	void(Generator::*OP_Table_FX1_[15])(){ &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_FX15, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_FX18, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_NULL, &Generator::OP_FX1E };
	void OP_FX1_();

	void OP_FX29(); //"FX29", "VI = FONT @ V[X]", { 0x0F00 }); }
	void OP_FX33(); //"FX33", "VI = BCD(V[X])", { 0x0F00 }); }
	void OP_FX55(); //"FX55", "Put V0-[X] into MEMORY @ VI", { 0x0F00 }); }
	void OP_FX65(); //"FX65", "Put MEMORY @ VI into V0-[X]", { 0x0F00 }); }

	void (Generator::*OP_Table_FX__[7])(){ &Generator::OP_FX0_, &Generator::OP_FX1_, &Generator::OP_FX29, &Generator::OP_FX33, &Generator::OP_NULL, &Generator::OP_FX55, &Generator::OP_FX65 };
	void OP_FX__();

	void (Generator::*OP_Table_F000[16])(){ &Generator::OP_0___, &Generator::OP_1NNN, &Generator::OP_2NNN, &Generator::OP_3XNN, &Generator::OP_4XNN, &Generator::OP_5XY0, &Generator::OP_6XNN, &Generator::OP_7XNN, &Generator::OP_8XY_, &Generator::OP_9XY0, &Generator::OP_ANNN, &Generator::OP_BNNN, &Generator::OP_CXNN, &Generator::OP_DXYN, &Generator::OP_EX__, &Generator::OP_FX__ };

public:
	unsigned short opID;
	void Generate(); //Call to parse AST and generate code

	Generator(Ast* e);
	~Generator();
};
#endif

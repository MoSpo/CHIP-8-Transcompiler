#include "main.h"
#include <initializer_list>
#include <sstream>

using namespace std;

string OP_Eval(const string& operationName, const string& operationDescription, const initializer_list<unsigned int>& operands) {
    string out = "";

    if (STDASM) {
        out += operationName;
        for (unsigned short op : operands) {
            int i = 0;
            while (((op >> i) & 0x1) == 0) {
                i += 4;
            }
            stringstream stream;
            stream << ((word & op) >> i);
            out += " " + stream.str();
        }

        if (VERBOSE) { 
            int l = out.length();
            for(int i = 0; i < 15-l; i++) out += " ";
            out += "//" + operationDescription;
        }
    }
    else {
        stringstream stream;
        stream << uppercase << hex << word;
        out += stream.str();
    }
    return out + "\n";
}

string OP_NULL() { 
    stringstream stream;
    stream << uppercase << hex << word;
    return "NULL (" + stream.str() + ")\n";
} //Function Array Pointer Padding

string OP_FX15() { return OP_Eval("FX15", "DELAY_TIMER = V[X]", { 0x0F00 }); }
string OP_FX18() { return OP_Eval("FX18", "SOUND_TIMER = V[X]", { 0x0F00 }); }
string OP_FX1E() { return OP_Eval("FX1E", "VI += V[X] w/ VF set in overflow", { 0x0F00 }); }

string(*OP_Table_FX1_[15])() = { OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_FX15, OP_NULL, OP_NULL, OP_FX18, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_FX1E };

string OP_8XY0() { return OP_Eval("8XY0", "V[X] = V[Y]", { 0x0F00, 0x00F0 }); }
string OP_8XY1() { return OP_Eval("8XY1", "V[X] |= V[Y]", { 0x0F00, 0x00F0 }); }
string OP_8XY2() { return OP_Eval("8XY2", "V[X] &= V[Y]", { 0x0F00, 0x00F0 }); }
string OP_8XY3() { return OP_Eval("8XY3", "V[X] ^= V[Y]", { 0x0F00, 0x00F0 }); }
string OP_8XY4() { return OP_Eval("8XY4", "V[X] += V[Y] w/ VF set in overflow", { 0x0F00, 0x00F0 }); }
string OP_8XY5() { return OP_Eval("8XY5", "V[X] -= V[Y] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
string OP_8XY6() { return OP_Eval("8XY6", "V[X] >>= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }
string OP_8XY7() { return OP_Eval("8XY7", "V[X] = V[Y] - V[X] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
string OP_8XYE() { return OP_Eval("8XYE", "V[X] <<= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }

string(*OP_Table_8XY_[15])() = { OP_8XY0, OP_8XY1, OP_8XY2, OP_8XY3, OP_8XY4, OP_8XY5, OP_8XY6, OP_8XY7, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_NULL, OP_8XYE };

string OP_FX0_() {
    if ((word & 0x000F) == 0x0007) return OP_Eval("FX07", "V[X] = DELAY_TIMER", { 0x0F00 });
    else return OP_Eval("FX0A", "Wait For Keypress, V[X] = KEY_VALUE", { 0x0F00 });
}

string OP_FX1_() { return (*(OP_Table_FX1_[word & 0x000F]))(); } //FX15, FX18, FX1E

string OP_FX29() { return OP_Eval("FX29", "VI = FONT @ V[X]", { 0x0F00 }); }
string OP_FX33() { return OP_Eval("FX33", "VI = BCD(V[X])", { 0x0F00 }); }
string OP_FX55() { return OP_Eval("FX55", "Put V0-[X] into MEMORY @ VI", { 0x0F00 }); }
string OP_FX65() { return OP_Eval("FX65", "Put MEMORY @ VI into V0-[X]", { 0x0F00 }); }

string(*OP_Table_FX__[7])() = { OP_FX0_, OP_FX1_, OP_FX29, OP_FX33, OP_NULL, OP_FX55, OP_FX65 };

string OP_0___() {
    if ((word & 0x000F) == 0x0000) return OP_Eval("00E0", "Clear the display", {});
    else return OP_Eval("00EE", "Return from subroutine", {});
}

string OP_1NNN() { return OP_Eval("1NNN", "PC = NNN", { 0x0FFF }); }
string OP_2NNN() { return OP_Eval("2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
string OP_3XNN() { return OP_Eval("3XNN", "If V[X] == NNN, skip next instruction", { 0x0F00, 0x00FF }); }
string OP_4XNN() { return OP_Eval("4XNN", "If V[X] != NNN, skip next instruction", { 0x0F00, 0x00FF }); }
string OP_5XY0() { return OP_Eval("5XY0", "If V[X] == V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
string OP_6XNN() { return OP_Eval("6XNN", "V[X] = NN", { 0x0F00, 0x00FF }); }
string OP_7XNN() { return OP_Eval("7XNN", "V[X] += NN", { 0x0F00, 0x00FF }); }

string OP_8XY_() { return (*(OP_Table_8XY_[word & 0x000F]))(); }

string OP_9XY0() { return OP_Eval("9XY0", "If V[X] != V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
string OP_ANNN() { return OP_Eval("ANNN", "VI = NNN", { 0x0FFF }); }
string OP_BNNN() { return OP_Eval("BNNN", "PC = NNN + V0", { 0x0FFF }); }
string OP_CXNN() { return OP_Eval("CXNN", "V[X] = RAND(range:00-FF) & NN", { 0x0F00, 0x00FF }); }
string OP_DXYN() { return OP_Eval("DXYN", "Draw to screen", { 0x0F00, 0x00F0, 0x000F }); }

string OP_EX__() {
    if ((word & 0x000F) == 0x0001) return OP_Eval("EXA1", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });
    else return OP_Eval("EX9E", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });
}

string OP_FX__() { return (*(OP_Table_FX__[(word & 0x00F0) >> 4]))(); }

string(*OP_Table_F000[16])() = { OP_0___, OP_1NNN, OP_2NNN, OP_3XNN, OP_4XNN, OP_5XY0, OP_6XNN, OP_7XNN, OP_8XY_, OP_9XY0, OP_ANNN, OP_BNNN, OP_CXNN, OP_DXYN, OP_EX__, OP_FX__ };

string OP_Lex() {
    return (*(OP_Table_F000[(word & 0xF000) >> 12]))();
}


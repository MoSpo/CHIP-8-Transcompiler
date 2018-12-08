#include "main.h"
#include <fstream>

bool VERBOSE;
bool STDASM;

unsigned short word;
unsigned char program[3696];

using namespace std;

bool Error(const char msg[]) {
    std::cout << msg << std::endl;
    return false;
}

bool OpenInputFile(int argc, char *argv[]) {
    if (argc <= 1) return Error("No file");
    else {
        ifstream file;
        file.open(argv[argc-1], std::ios::binary);
        if (!file) return Error("Unable to open file");

        int i = 0;

        while(true) {
            file.read(reinterpret_cast<char*>(&program[i]), 1);
            file.read(reinterpret_cast<char*>(&program[i+1]), 1);
            i+=2;
            if (!file) break;
        }
        
        file.close();
        
        for (; i < 3696; i++) program[i] = 0;
        
    }
    return true;
}

bool ReadFlags(int argc, char *argv[]) {
    int i = 1;
    int j = 1;
    while(i < argc-1) {
        if (argv[i][0] == '-') {
            switch (argv[i][j]) {
            case 'v':
                VERBOSE = true;
                j++;
                break;

            case 's':
                STDASM = true;
                j++;
                break;

            case '\0':
                if (j == 1) return Error("Unrecognised parameters");
                j = 1;
                i++;
                break;

            default:
                return Error("Unrecognised parameters");
            }
        } else return Error("Corrupt parameters");
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (OpenInputFile(argc, argv)) {
        if (!ReadFlags(argc, argv)) return -1;

        ofstream file;
        string str(argv[argc-1]);
        file.open(str + "asm");
        int PC = 0;

        while (true) {
            word = (program[PC] << 8) | (program[PC + 1]);
            if (word == 0) break;
            file << OP_Lex();
            PC+=2;
        }

        file.close();

    } else return -1;
    return 0;
}

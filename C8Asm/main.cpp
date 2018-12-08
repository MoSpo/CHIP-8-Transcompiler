#include "main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

using namespace std;

int Error(const char msg[]) {
	std::cout << msg << std::endl;
	return -1;
}


int main(int argc, char *argv[]) {
	if (argc <= 1) return Error("No file");
	else {
		ifstream infile;
		infile.open(argv[argc - 1]);
		if (!infile) return Error("Unable to open file");

		ofstream file;
		string str(argv[argc - 1]);
		file.open(str.substr(0, str.size() - 3), ios::binary);

		for (string line; getline(infile, line); ) {
			size_t rest = line.find(" ");
			if (rest != string::npos) {
				return Error("ASM layout file - Unable to parse (yet)");
			} else {
				uint16_t word = stoi(line, 0, 16);
				char fst = (word & 0xFF00) >> 8;
				char snd = word & 0x00FF;
				file.write((char*)&fst, sizeof(char));
				file.write((char*)&snd, sizeof(char));
			}
		}
		infile.close();
		file.close();

	}
	return 0;
}

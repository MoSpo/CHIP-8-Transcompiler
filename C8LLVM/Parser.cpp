#include "Parser.h"
#include "Ast.h"
#include "Flags.h"

using namespace std;

Ast* Parser::OP_Eval(const string& operationName, const string& operationDescription, const vector<unsigned int>& operandMasks) {

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

Ast* Parser::OP_NULL() {
    return NULL; //TODO: Handling data
}

Ast* Parser::OP_0___() {
	if(DEBUG && (word & 0x00F0) == 0x00B0) return OP_Eval("00BX", "print V[X] (Debug only)", { 0x000F });
	else if ((word & 0x000F) == 0x0000) return OP_Eval("00E0", "Clear the display", {});
	else return OP_Eval("00EE", "Return from subroutine", {});
}

Ast* Parser::OP_1NNN() { return OP_Eval("1NNN", "PC = NNN", { 0x0FFF }); }
Ast* Parser::OP_2NNN() { return OP_Eval("2NNN", "Push PC to Stack, PC = NNN", { 0x0FFF }); }
Ast* Parser::OP_3XNN() { return OP_Eval("3XNN", "If V[X] == NNN, skip next instruction", { 0x0F00, 0x00FF }); }
Ast* Parser::OP_4XNN() { return OP_Eval("4XNN", "If V[X] != NNN, skip next instruction", { 0x0F00, 0x00FF }); }
Ast* Parser::OP_5XY0() { return OP_Eval("5XY0", "If V[X] == V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_6XNN() { return OP_Eval("6XNN", "V[X] = NN", { 0x0F00, 0x00FF }); }
Ast* Parser::OP_7XNN() { return OP_Eval("7XNN", "V[X] += NN", { 0x0F00, 0x00FF }); }

Ast* Parser::OP_8XY0() { return OP_Eval("8XY0", "V[X] = V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XY1() { return OP_Eval("8XY1", "V[X] |= V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XY2() { return OP_Eval("8XY2", "V[X] &= V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XY3() { return OP_Eval("8XY3", "V[X] ^= V[Y]", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XY4() { return OP_Eval("8XY4", "V[X] += V[Y] w/ VF set in overflow", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XY5() { return OP_Eval("8XY5", "V[X] -= V[Y] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XY6() { return OP_Eval("8XY6", "V[X] >>= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XY7() { return OP_Eval("8XY7", "V[X] = V[Y] - V[X] w/ VF set in no underflow", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_8XYE() { return OP_Eval("8XYE", "V[X] <<= 1  w/ VF set to lost bit", { 0x0F00, 0x00F0 }); }

Ast* Parser::OP_8XY_() { return (this->*OP_Table_8XY_[word & 0x000F])(); }

Ast* Parser::OP_9XY0() { return OP_Eval("9XY0", "If V[X] != V[Y], skip next instruction", { 0x0F00, 0x00F0 }); }
Ast* Parser::OP_ANNN() { return OP_Eval("ANNN", "VI = NNN", { 0x0FFF }); }
Ast* Parser::OP_BNNN() { return OP_Eval("BNNN", "PC = NNN + V0", { 0x0FFF }); } //TODO LATER
Ast* Parser::OP_CXNN() { return OP_Eval("CXNN", "V[X] = RAND(range:00-FF) & NN", { 0x0F00, 0x00FF }); }
Ast* Parser::OP_DXYN() { return OP_Eval("DXYN", "Draw to screen", { 0x0F00, 0x00F0, 0x000F }); }

Ast* Parser::OP_EX__() {
	if ((word & 0x000F) == 0x0001) return OP_Eval("EXA1", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });
	else return OP_Eval("EX9E", "If KEY[V[X]] not pressed, skip next instruction", { 0x0F00 });
}

Ast* Parser::OP_FX0_() {
    if ((word & 0x000F) == 0x0007) return OP_Eval("FX07", "V[X] = DELAY_TIMER", { 0x0F00 });
    else return OP_Eval("FX0A", "Wait For Keypress, V[X] = KEY_VALUE", { 0x0F00 });
}

Ast* Parser::OP_FX15() { return OP_Eval("FX15", "DELAY_TIMER = V[X]", { 0x0F00 }); }
Ast* Parser::OP_FX18() { return OP_Eval("FX18", "SOUND_TIMER = V[X]", { 0x0F00 }); }
Ast* Parser::OP_FX1E() { return OP_Eval("FX1E", "VI += V[X] w/ VF set in overflow", { 0x0F00 }); }

Ast* Parser::OP_FX1_() { return (this->*OP_Table_FX1_[word & 0x000F])(); } //FX15, FX18, FX1E

Ast* Parser::OP_FX29() { return OP_Eval("FX29", "VI = FONT @ V[X]", { 0x0F00 }); }
Ast* Parser::OP_FX33() { return OP_Eval("FX33", "VI = BCD(V[X])", { 0x0F00 }); }
Ast* Parser::OP_FX55() { return OP_Eval("FX55", "Put V0-[X] into MEMORY @ VI", { 0x0F00 }); }
Ast* Parser::OP_FX65() { return OP_Eval("FX65", "Put MEMORY @ VI into V0-[X]", { 0x0F00 }); }

Ast* Parser::OP_FX__() { return (this->*OP_Table_FX__[(word & 0x00F0) >> 4])(); }

std::vector<unsigned short> Parser::BR_NULL(unsigned short Index) {
	//THIS SHOULD NEVER BE CALLED!
    return std::vector<unsigned short>{};
}

std::vector<unsigned short> Parser::BR_00EE(unsigned short Index) {
	if (word != 0x00EE) return std::vector<unsigned short>{};
	unsigned short lnext = Index + 2;
	return std::vector<unsigned short>{lnext};
}

std::vector<unsigned short> Parser::BR_1NNN(unsigned short Index) {
	unsigned short lnext = Index + 2;
	unsigned short ljump = word & 0x0FFF;
	return std::vector<unsigned short>{lnext, ljump};
}

std::vector<unsigned short> Parser::BR_2NNN(unsigned short Index) {
	unsigned short lcall= word & 0x0FFF;
	return std::vector<unsigned short>{lcall};
}

std::vector<unsigned short> Parser::BR_IF(unsigned short Index) {
	unsigned short lelse = Index + 2;
	unsigned short lif = Index + 4;
	return std::vector<unsigned short>{lelse, lif};
}

std::vector<unsigned short> Parser::BR_BNNN(unsigned short Index) {
	std::vector<unsigned short> vec{};
	for (unsigned short i = 1; i < 256; i++) {
		vec.push_back(Index + 2*i);
	}
	return vec;
}

unsigned short Parser::FindDataPartition() {
	unsigned int PC = 0;
	unsigned int lowestBound = programLength;
	while (PC < lowestBound) { //Loop for whole binary
		word = (program[PC] << 8) | (program[PC + 1]);
		if ((word & 0xF000) == 0xA000) { //if ANNN
			unsigned short bound = word & 0x0FFF;
			if (bound - 512 < lowestBound) lowestBound = bound - 512; //PC will never get into data as the correct bound will be found by the time you exit the code
		} //also need to subtract the 512 as ANNN addresses will be relative to the interpreter gap
		PC += 2;
	}
	return lowestBound;
}

std::vector<unsigned char> Parser::GetData(unsigned short dataPartition) {
	unsigned int PC = dataPartition;
	std::vector<unsigned char> data;
	while (PC < programLength) { //Loop on data
		data.push_back(program[PC]);
		PC++;
	}
	/*while (PC < 4096) { //Add memory padding
		data.push_back(0);
		PC++;
	}*/
	return data;
}

std::map<unsigned short, BasicBlock*> Parser::CreateBlocks(unsigned short dataPartition) {
	unsigned int PC = 0;

	std::map<unsigned short, BasicBlock*> blocks;
	BasicBlock* mainEntryBlock = new BasicBlock{0};
	mainEntryBlock->isFunctionBlock = true;
	blocks[0] = mainEntryBlock;

	while (PC < dataPartition) {
		word = (program[PC] << 8) | (program[PC + 1]);
		std::vector<unsigned short> blockAddresses = (this->*BR_Table_F000[(word & 0xF000) >> 12])(PC);
		for (unsigned short adr : blockAddresses) {
			if (blocks.find(adr) == blocks.end()) {
				BasicBlock* b = new BasicBlock{ adr };
				b->isFunctionBlock = (word & 0xF000) == 0x2000;
				blocks[adr] = b;
			}
		}
		PC += 2;
	}

	return blocks;
}

std::map<unsigned short, BasicBlock*> Parser::LinkBlocks(std::map<unsigned short, BasicBlock*> blocksWithAddressID) {
/*	for (auto const&blockMap : blocksWithAddressID) {
		BasicBlock* block = blockMap.second; //get blocks from map
		unsigned short prevKey = 0;
		for (unsigned short adr : block->entryAddresses) {
			for (auto const&[key, val] : blocksWithAddressID) {
				if (adr == key -2) { // the end of a block NOT TRUE COULD BE A BLOCK GENERATED BY IF OR SWITCH
					block->entryBlockLinks.push_back(blocksWithAddressID[prevKey]);
					block->entryCount++;
					blocksWithAddressID[prevKey]->exitBlockLinks.push_back(block);
					blocksWithAddressID[prevKey]->exitCount++;
					break;
				} else if (adr >= prevKey && adr < key) { // in the middle of a block, a function call
					block->entryBlockLinks.push_back(blocksWithAddressID[prevKey]);
					block->entryCount++;
					blocksWithAddressID[prevKey]->exitBlockLinks.push_back(block);
					blocksWithAddressID[prevKey]->exitCount++;
					break;
				}
				prevKey = key;
			}
			prevKey = 0;
		}
	}

	for (auto const&blockMap : blocksWithAddressID) {
		BasicBlock* block = blockMap.second;
		if (block->entryCount == 0) { //Potentially remove unreachable code here? Though shouldn't be any
		}
		if (block->exitCount == 1) {
			BasicBlock* linkedBlock = block->exitBlockLinks[0];
			if (linkedBlock->entryCount == 1) {
				block->MergeIntoBlock(linkedBlock); //TODO
			}
		}
	}*/
}
	//TODO: nodes clearly need concepts of where they are meant to jump to
	//concept: nodes have list of blocks to branch to + function blocks to call to
	//if a node is not meant to have branch, on block generation add the link
	//if a node is meant to have branch, on block generation remove all branches already on the block, as they wont be used, add the links the branch specially uses, and set a flag hasExplicitBranch, meaning no more branches will be added to it
	//at code generation, if the node has hasExplicitBranch == false, generate extra branch afterwards. If it == true, don't generate branches
	//function calls are not effected and do not count as branches.
std::vector<BasicBlock*> Parser::FillBlocks(std::map<unsigned short, BasicBlock*> emptyBlocks, unsigned short dataPartition) {
	std::vector<unsigned short> blockAddresses;
	std::vector<BasicBlock*> blocks;
	for (auto const&blockMap : emptyBlocks) {
		blockAddresses.push_back(blockMap.first);
		blocks.push_back(blockMap.second);
	}

	Ast* Current;
	unsigned int PC = 0;
	unsigned short prevWord;
	unsigned short blockIndex = 0;

	while (PC < dataPartition) { //Loop for the rest of the code

		word = (program[PC] << 8) | (program[PC + 1]);
		if (PC == blockAddresses[blockIndex]) {
			if (blockIndex != 0) { //or if Current != null, i.e. finishing off the last block
				unsigned short hi = prevWord & 0xF000;
				if (hi == 0x1000) { //jump
					blocks[blockIndex - 1]->exitBlockLinks.clear();
					blocks[blockIndex - 1]->exitBlockLinks.push_back(emptyBlocks[prevWord & 0xFFF - 512]);
					blocks[blockIndex - 1]->hasExplicitBranch = true;
				} else if (hi == 0x2000) { //call
					blocks[blockIndex - 1]->functionBlockLinks.push_back(emptyBlocks[prevWord & 0xFFF - 512]);
				} else if (hi == 0xB000) { //indirect
					//TODO: later
				} else if (hi == 0x3000 || hi == 0x4000 || hi == 0x5000 || hi == 0x9000 || hi ==  0xE000) { //if
					blocks[blockIndex - 1]->exitBlockLinks.clear();
					blocks[blockIndex - 1]->exitBlockLinks.push_back(emptyBlocks[PC]);
					blocks[blockIndex - 1]->exitBlockLinks.push_back(emptyBlocks[PC+2]);
					blocks[blockIndex - 1]->hasExplicitBranch = true;
					if(!emptyBlocks[PC]->hasExplicitBranch) emptyBlocks[PC]->exitBlockLinks.push_back(emptyBlocks[PC+2]);
				} else if (prevWord == 0x00EE) { //ret
					blocks[blockIndex - 1]->exitBlockLinks.clear();
					blocks[blockIndex - 1]->hasExplicitBranch = true;
				}
			}
			Current = (this->*OP_Table_F000[(word & 0xF000) >> 12])(); //Setup first node of basicblock
			blocks[blockIndex]->code = Current;
			blockIndex++;
			PC += 2;
		} else {
			Current->nextInstruction = (this->*OP_Table_F000[(word & 0xF000) >> 12])(); //Rest of block
			Current = Current->nextInstruction;
			PC += 2;
		}
		prevWord = word;
	}

	unsigned short hi = prevWord & 0xF000;
	if (hi == 0x1000) { //jump
		blocks[blockIndex - 1]->exitBlockLinks.clear();
		blocks[blockIndex - 1]->exitBlockLinks.push_back(emptyBlocks[prevWord & 0xFFF - 512]);
		blocks[blockIndex - 1]->hasExplicitBranch = true;
	}
	else if (hi == 0x2000) { //call
		blocks[blockIndex - 1]->functionBlockLinks.push_back(emptyBlocks[prevWord & 0xFFF - 512]);
	}
	else if (prevWord == 0x00EE) { //ret
		blocks[blockIndex - 1]->exitBlockLinks.clear();
		blocks[blockIndex - 1]->hasExplicitBranch = true;
	}

	return blocks;
}

std::vector<BasicBlock*> Parser::ParseCode(unsigned short dataPartition) {
	return FillBlocks(CreateBlocks(dataPartition), dataPartition);
}

std::vector<unsigned char> Parser::ParseData(unsigned short dataPartition) {
	return GetData(dataPartition);
}

Ast* Parser::SimpleParseInput() {
	unsigned int PC = 0;
	word = (program[PC] << 8) | (program[PC + 1]);
	Ast* Entry = (this->*OP_Table_F000[(word & 0xF000) >> 12])(); //Setup first node
	Ast* Current = Entry;
	PC += 2;
	while (PC < programLength) { //Loop for the rest of the program
		word = (program[PC] << 8) | (program[PC + 1]);
		Current->nextInstruction = (this->*OP_Table_F000[(word & 0xF000) >> 12])();
		Current = Current->nextInstruction;
		PC += 2;
	}
	return Entry;
}

Parser::Parser(unsigned char p[], unsigned int l) {
	program = p;
	programLength = l;
};

Parser::~Parser() {};

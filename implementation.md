# Implementation

- File is read into an array and passed into the parser

##Parser
- Pass 1: Finds the code-data split. Finds all ANNN instructions and uses their operands to find the lowest bound for the data split.
- Pass 2: Loads data into a vector.
- Pass 3: Creates basic block headers by finding any branching or conditional instructions. Also marks function call headers. Keeps a map between address and header so footers can be found and code can be loaded into them later.
- Pass 4: Fills blocks with code and links blocks with one another. Sets explicitBranch flags if a block ends in a branch instruction for later generation.
- The blocks and the data vector are passed onto the generator.

##Generator
- The generator sets up llvm module and initialises registers and memory from the data vector.
- Declares external functions and sets up functions for each function block in a map from basic block to function.
- Passes over each function block and keeps a FIFO queue to all referenced blocks to be generated later. When the FIFO queue is consumed and completely generated,  the next function block is loaded.
- Code in a block is looped through and LLVM is generated. Branches will be added at the end of blocks without an explicitBranch.
- Generated code then outputted to stdout. 
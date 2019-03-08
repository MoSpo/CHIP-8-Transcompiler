# C8LLVM

## How to use the C8LLVM project

`make -C C8LLVM`

`./C8LLVM/C8LLVM Tests/MAZE > Output/game.ll`

`make -C Output`

1. Run make. C8LLVM depends on the LLVM libraries which can be built from: http://releases.llvm.org/download.html
(Follow the guides below if you need help)

https://llvm.org/docs/CMake.html
https://llvm.org/docs/GettingStartedVS.html

2. Run C8LLVM by passing it a CHIP-8 binary file. If you want to test your own programs, you will need to write plaintext files such as the ones in the Test folder. Build the C8ASM project which will convert your plaintext file into a binary which can be given to C8LLVM. C8LLVM will then output LLVM IR to stdout, which can be piped to a file. The makefile provided uses clang to allow compatability with llvm-config.

3. Create an object file with the output using llc (part of the LLVM library) and link that with IOFIle.cpp and IOFile.h found in the Output folder. IOFile.cpp depends on SDL2.h:
https://www.libsdl.org/

4. Run produced file.

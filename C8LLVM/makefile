CXX=clang++
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LLVMFLAGS=$(shell /home/ls/Documents/Projects/llvm/llvm-7.0.1.src/build/bin/llvm-config --cxxflags --ldflags --system-libs --libs core)
LDLIBS=$(shell root-config --libs)

SRCS=Generator.cpp Parser.cpp Ast.cpp Main.cpp
HDRS= Generator.h Parser.h Ast.h Lib.h Flags.h 
OBJS=$(subst .cpp,.o,$(SRCS))

C8LLVM: $(SRCS) $(HDRS)
	$(CXX) -g $(SRCS) -I. $(LLVMFLAGS) -o C8LLVM

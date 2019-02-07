#ifndef LIB_H
#define LIB_H
#ifdef _WIN32
// Windows includes
#include "llvm\IR\Module.h"
#include "llvm\IR\Function.h"
#include "llvm\IR\Verifier.h"
#include "llvm\IR\IRBuilder.h"
#include "llvm\IR\DerivedTypes.h"
#endif
#ifdef __unix__
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#endif
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#endif

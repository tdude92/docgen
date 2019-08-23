#ifndef DGPARSERS_HPP
#define DGPARSERS_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "dgtypes.hpp"

// Functions
LANG::Enum                  getFileType(const std::string &fileName);

// Python docgen.
std::vector<Instruction>    getInstructions_py(const std::string fileName);
std::vector<std::string>    parseInstruction_py(Instruction instruction);
Arg                         parseFuncArg_py(std::string expression);
std::string          parseDesc_py(std::string desc);

#endif
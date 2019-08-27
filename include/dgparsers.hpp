#ifndef DGPARSERS_HPP
#define DGPARSERS_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "dgtypes.hpp"

// Functions
LANG::Enum                  getFileType(const std::string &fileName);

// Parsers
std::vector<std::string>   *parseInstruction(Instruction instruction);
Arg                         parseFuncArg(std::string expression);
std::string                 parseDesc(std::string desc);

#endif
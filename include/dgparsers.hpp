#ifndef DGPARSERS_HPP
#define DGPARSERS_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "dgtypes.hpp"

// Functions
LANG::Enum getFileType(const std::string &fileName);
std::vector<std::string> getInstructions_py(std::ifstream &file);

#endif
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>

#include "dgtypes.hpp"
#include "dgparsers.hpp"

// TODO: Make sure files of type TYPE::OTHER are skipped.

LANG::Enum getFileType(const std::string &fileName) {
    std::string fileExt = "";

    // Determine file type.
    for (auto c = fileName.end() - 1;
         *c != '.'  &&
         *c != '/'  &&
         *c != '\\';
        --c) {
        fileExt += *c;
        if (c == fileName.begin()) {
            // This if statement is needed to allow
            // the first character of fileName
            // to be appended to fileExt.
            break;
        }
    }
    std::reverse(fileExt.begin(), fileExt.end());

    if (fileExt == "py") {
        return LANG::PYTHON;
    } else if (fileExt == "cpp" ||
               fileExt == "cc"  ||
               fileExt == "cxx" ||
               fileExt == "hpp" ||
               fileExt == "hh"  ||
               fileExt == "hxx") {
        return LANG::CPP;
    } else {
        std::cerr << fileName << " is of an unsupported file type. Skipping..." << std::endl;
        return LANG::OTHER;
    }
}


std::vector<std::string> getInstructions_py(std::ifstream &file) {
    std::vector<std::string> instructions;

    std::string line;
    std::string instruction;
    bool inBlock = false; // Check if the line is part of a block (between DES_BEG/DESC_END and ARGS_BEG/ARGS_END)
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(' ')); // Trim leading whitespace.
        if (line.substr(0, 3) == "#dg") {
            instruction = line.substr(4, line.npos);
            instructions.push_back(instruction);

            // Check when a block begins.
            if (instruction == "DESC_BEG" || instruction == "ARGS_BEG") {
                inBlock = true;
            } else if (instruction == "DESC_END" || instruction == "ARGS_END") {
                inBlock = false;
            }
        } else if (inBlock) {
            instruction = line.substr(1, line.npos);
            instruction.erase(0, instruction.find_first_not_of(' ')); // Trim leading whitespace in blocks.
            instructions.push_back(instruction);
        }
    }
    
    return instructions;
}

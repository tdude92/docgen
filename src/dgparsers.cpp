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

std::vector<std::string> getInstructions_py(std::ifstream file) {
    std::vector<std::string> instructions;

    std::string line;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(' ')); // Trim leading whitespace.
        if (line.substr(0, 3) == "#dg") {
            instructions.push_back(line.substr(4, line.npos));
        }
    }
    
    return instructions;
}

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>

#include "dgparsers.hpp"

int getFileType(const std::string &fileName) {
    std::string fileExt = "";

    // Determine file type.
    for (auto c = fileName.end() - 1;
         *c != '.'  &&
         *c != '/'  &&
         *c != '\\';
        --c) {
        fileExt += *c;
        if (c == fileName.begin()) {
            // This if statement needed to allow
            // the first character of fileName
            // to be appended to fileExt.
            break;
        }
    }
    std::reverse(fileExt.begin(), fileExt.end());

    if (fileExt == "py") {
        return PYTHON;
    } else if (fileExt == "cpp" ||
               fileExt == "cc"  ||
               fileExt == "cxx" ||
               fileExt == "hpp" ||
               fileExt == "hh"  ||
               fileExt == "hxx") {
        return CPP;
    } else {
        std::cerr << "(getFileType) Error: unsupported file type." << std::endl;
        exit(EXIT_FAILURE);
    }
}
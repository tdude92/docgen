#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

#include "dgparsers.hpp"

int main(int argc, char *argv[]) {
    // Specify how to use docgen.
    if (argc == 1) {
        std::cerr << "Usage: docgen [filename].[extension]" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string fileName = argv[1];
    LANG::Enum fileType = getFileType(fileName);

    //std::vector<FuncDoc> functionDocumentation;
    //std::vector<ClassDoc> classDocumentation;
    std::ifstream file(fileName);
    std::vector<std::string> instructions = getInstructions_py(file);
    for (auto s : instructions) {
        std::cout << s << std::endl;
    }

    file.close();
    return 0;
}
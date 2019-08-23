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
    std::vector<Instruction> instructions = getInstructions_py(fileName);
    std::vector<std::string> tokens;
    for (auto s : instructions) {
        tokens = parseInstruction_py(s);
        for (auto token : tokens) {
            std::cout << token << std::endl;
        }
    }
    return 0;
}
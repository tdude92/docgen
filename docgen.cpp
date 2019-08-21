#include <iostream>
#include <fstream>
#include <cstdlib>

#include "dgparsers.hpp"

int main(int argc, char *argv[]) {
    // Specify how to use docgen.
    if (argc == 1) {
        std::cerr << "Usage: docgen [filename].[extension]" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string fileName = argv[1];
    int fileType = getFileType(fileName);

    std::cout << fileType << std::endl;

    std::ifstream file(fileName);
    file.close();
    return 0;
}
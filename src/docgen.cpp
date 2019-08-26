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

    ClassDoc x;

    std::cout << x.name() << std::endl;
    return 0;
}
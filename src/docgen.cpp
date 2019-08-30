#include <iostream>
#include <cstdlib>

#include "dgparsers.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: docgen \"your project name\"" << std::endl
                  << "   or  docgen yourprojectname"       << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string projectName = argv[1];
    genDoc_project(projectName);

    return 0;
}
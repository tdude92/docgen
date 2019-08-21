#ifndef DGPARSERS_HPP
#define DGPARSERS_HPP

enum {
    PYTHON,
    CPP
};

int getFileType(const std::string &fileName);

#endif
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <cstdlib>

#include "dgtypes.hpp"
#include "dgparsers.hpp"

// TODO: Make sure files of type TYPE::OTHER are skipped.
// TODO: Use invalidInstructionsFlag.

bool invalidInstructionsFlag = false;

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


// Python functions.
std::vector<Instruction> getInstructions_py(const std::string fileName) {
    std::vector<Instruction> instructions;

    std::ifstream file(fileName);

    std::string line;
    std::string instruction;
    bool inBlock = false; // Check if the line is part of a block (between DES_BEG/DESC_END and ARGS_BEG/ARGS_END)
    for (int lineNo = 1; std::getline(file, line); ++lineNo) {
        line.erase(0, line.find_first_not_of(' ')); // Trim leading whitespace.
        line.erase(line.find_last_not_of(' ') + 1);
        if (line.substr(0, 3) == "#dg") {
            instruction = line.substr(4, line.npos);
            instructions.push_back(Instruction(fileName, lineNo, instruction));

            // Check when a block begins.
            if (instruction == "DESC_BEG" || instruction == "ARGS_BEG") {
                inBlock = true;
            } else if (instruction == "DESC_END" || instruction == "ARGS_END") {
                inBlock = false;
            }
        } else if (inBlock) {
            instruction = line.substr(1, line.npos);
            instruction.erase(0, instruction.find_first_not_of(' ')); // Trim leading whitespace in blocks.
            instructions.push_back(Instruction(fileName, lineNo, instruction));
        }
    }
    
    file.close();
    return instructions;
}

inline void trimKeyword(std::string &instruction, const std::string keyword) {
    // Trim a keyword from an instruction.
    // A helper function for parsing.
    instruction.erase(0, keyword.length());
    instruction.erase(0, instruction.find_first_not_of(' '));
}
std::vector<std::string> parseInstruction_py(Instruction instruction) {
    // Return a vector of tokens to make executing instructions simpler.
    std::vector<std::string> parsedInstruction;

    if (instruction.instruction.substr(0, 5) == "START") {
        parsedInstruction.push_back("START");
        trimKeyword(instruction.instruction, "START");

        parsedInstruction.push_back(instruction.instruction);
    } else if (instruction.instruction.substr(0, 3) == "END") {
        parsedInstruction.push_back("END");
        trimKeyword(instruction.instruction, "END");

        parsedInstruction.push_back(instruction.instruction);
    } else if (instruction.instruction.substr(0, 7) == "RETURNS") {
        parsedInstruction.push_back("RETURNS");
        trimKeyword(instruction.instruction, "RETURNS");

        // Iterate through all provided possible return
        // values and append them to parsedInstruction.
        char instruction_cstr[instruction.instruction.length() + 1];
        strcpy(instruction_cstr, instruction.instruction.c_str());

        char* returnType;
        returnType = strtok(instruction_cstr, " ");

        while (returnType != NULL) {
            parsedInstruction.push_back(returnType);
            returnType = strtok(NULL, " ");
        }
    } else if (instruction.instruction.substr(0, 5) == "BRIEF") {
        parsedInstruction.push_back("BRIEF");
        trimKeyword(instruction.instruction, "BRIEF");

        parsedInstruction.push_back(instruction.instruction);
    } else {
        std::cerr << "(" << instruction.fileName << ")Invalid instruction[" << instruction.lineNo
                  << "]: \"" << instruction.instruction << "\"" << std::endl;
        invalidInstructionsFlag = true;
    }

    return parsedInstruction;
}

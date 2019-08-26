#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdlib>

#include "dgtypes.hpp"
#include "dgexcept.hpp"
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
    } else {
        return LANG::OTHER;
    }
}


// Python functions.
std::vector<Instruction> getInstructions(const std::string fileName) {
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
            if (instruction == "START ARGS" || instruction == "ARGS_BEG") {
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
std::vector<std::string> *parseInstruction(Instruction instruction) {
    // Return a vector of tokens to make executing instructions simpler.
    std::vector<std::string> *parsedInstruction;
    parsedInstruction = new std::vector<std::string>;

    if (instruction.instruction.substr(0, 5) == "START") {
        parsedInstruction->push_back("START");
        trimKeyword(instruction.instruction, "START");

        parsedInstruction->push_back(instruction.instruction);
    } else if (instruction.instruction.substr(0, 3) == "END") {
        parsedInstruction->push_back("END");
        trimKeyword(instruction.instruction, "END");

        parsedInstruction->push_back(instruction.instruction);
    } else if (instruction.instruction.substr(0, 7) == "RETURNS") {
        parsedInstruction->push_back("RETURNS");
        trimKeyword(instruction.instruction, "RETURNS");

        // Iterate through all provided possible return
        // values and append them to parsedInstruction.
        char instruction_cstr[instruction.instruction.length() + 1];
        strcpy(instruction_cstr, instruction.instruction.c_str());

        char* returnType;
        returnType = strtok(instruction_cstr, " ");

        while (returnType != NULL) {
            parsedInstruction->push_back(returnType);
            returnType = strtok(NULL, " ");
        }
    } else if (instruction.instruction.substr(0, 5) == "BRIEF") {
        parsedInstruction->push_back("BRIEF");
        trimKeyword(instruction.instruction, "BRIEF");

        parsedInstruction->push_back(instruction.instruction);
    } else {
        std::cerr << "(" << instruction.fileName << ")Invalid instruction[" << instruction.lineNo
                  << "]: \"" << instruction.instruction << "\"" << std::endl;
        delete parsedInstruction;
        return nullptr;
    }

    return parsedInstruction;
}

Arg parseFuncArg(std::string expr) {
    Arg parsedArg;
    int startIndex, endIndex;

    expr.erase(0, expr.find_first_not_of(' ')); // Erase leading whitespace.
                                                // parseFuncArg_py does not take an input
                                                // from getInstructions_py, so it needs to
                                                // trim whitespace itself.

    startIndex = 0;
    endIndex = expr.find_first_of(')', startIndex) + 1;
    parsedArg.types = expr.substr(startIndex, endIndex);

    startIndex = endIndex;
    endIndex = expr.find_first_of(':', startIndex);
    parsedArg.name = expr.substr(startIndex, endIndex - startIndex);

    startIndex = endIndex + 1;
    parsedArg.desc = expr.substr(startIndex, expr.npos - startIndex);

    // Trim leading whitespace after delimiters.
    parsedArg.types.erase(0, parsedArg.types.find_first_not_of(' '));
    parsedArg.name.erase(0, parsedArg.name.find_first_not_of(' '));
    parsedArg.desc.erase(0, parsedArg.desc.find_first_not_of(' '));
    
    return parsedArg;
}

std::string parseDesc(std::string desc) {
    desc.erase(0, desc.find_first_not_of(' '));
    desc = desc.substr(1, desc.npos - 1);
    desc.erase(0, desc.find_first_not_of(' '));

    return desc;
}

template <typename T>
void freeDocList(std::vector<T*> vec) {
    // For freeing vectors of dynamically allocated
    // FuncDoc and ClassDoc objects in a vector.
    for (T* element : vec) {
        delete element;
    }
}

void genDoc_file(std::string fileName, std::vector<FuncDoc*> &funcBuf, std::vector<ClassDoc*> &classBuf) {
    // Concatenate the function/class docs in a file to buffers.
    try {
        if (getFileType(fileName) == LANG::OTHER)
            throw unsupported_filetype_error(fileName);
        else if (getFileType(fileName) == LANG::PYTHON) {
            std::ifstream file(fileName);
            std::string line;

            Instruction instruction;
            std::vector<std::string> *parsedInstruction;

            std::vector<ClassDoc*> classes;
            std::vector<FuncDoc*> functions;

            ClassDoc *currClass = nullptr;
            FuncDoc  *currFunc = nullptr;

            bool isArg = false, isDesc = false;

            for (int lineNo = 1; std::getline(file, line); ++lineNo) {
                // Use a for loop to keep track of line numbers.
                line.erase(0, line.find_first_not_of(' ')); // Trim leading whitespace.
                if (line.substr(0, 3) == "#dg") {
                    line = line.substr(4, line.npos);
                    instruction = Instruction(fileName, lineNo, line);

                    parsedInstruction = parseInstruction(instruction);
                    
                    if (parsedInstruction != nullptr) {
                        if ((*parsedInstruction)[0] == "START") {
                            if ((*parsedInstruction)[1] == "CLASS") {
                                currClass = new ClassDoc();
                            } else if ((*parsedInstruction)[1] == "FUNC") {

                            } else if ((*parsedInstruction)[1] == "ARGS") {
                                
                            } else if ((*parsedInstruction)[1] == "DESC") {
                                
                            } else {
                                throw dg_syntax_error("Invalid START type \"" + (*parsedInstruction)[1] + "\"", fileName, lineNo);
                            }
                        } else if ((*parsedInstruction)[0] == "END") {

                        } else if ((*parsedInstruction)[0] == "RETURNS") {
                            
                        } else if ((*parsedInstruction)[0] == "BRIEF") {
                            
                        }
                    } else if (isArg) {
                        if (currFunc == nullptr) {
                            throw dg_syntax_error("Args defined while not in function", fileName, lineNo);
                        }
                    } else if (isDesc) {
                        if (currFunc == nullptr && currClass == nullptr) {
                            throw dg_syntax_error("Desc defined while not in class or function", fileName, lineNo);
                        }
                    }
                }
            }

            // Append func/class docs to buffers.
            funcBuf.insert(funcBuf.end(), functions.begin(), functions.end());
            classBuf.insert(classBuf.end(), classes.begin(), classes.end());
        }
    } catch (const unsupported_filetype_error &err) {
        std::cerr << err.what() << std::endl;
    } catch (const dg_syntax_error &err) {
        std::cerr << err.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
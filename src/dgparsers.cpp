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
Arg parseFuncArg(std::string expr) {
    Arg parsedArg;
    size_t startIndex, endIndex;

    expr.erase(0, expr.find_first_of('#') + 1);

    startIndex = 0;
    endIndex = expr.find_first_of(')', startIndex) + 1;
    parsedArg.types = expr.substr(startIndex, endIndex);

    startIndex = endIndex;
    endIndex = expr.find_first_of(':', startIndex);
    parsedArg.name = expr.substr(startIndex, endIndex - startIndex);

    if (endIndex != expr.npos) {
        // If the description for the arg is not omitted.
        startIndex = endIndex + 1;
        parsedArg.desc = expr.substr(startIndex, expr.npos - startIndex);
    } else {
        parsedArg.desc = "";
    }

    // Trim leading whitespace after delimiters.
    parsedArg.types.erase(0, parsedArg.types.find_first_not_of(' '));
    parsedArg.name.erase(0, parsedArg.name.find_first_not_of(' '));
    parsedArg.desc.erase(0, parsedArg.desc.find_first_not_of(' '));
    
    return parsedArg;
}

std::string parseDesc(std::string desc) {
    desc.erase(0, desc.find_first_not_of(' '));
    desc = desc.substr(1, desc.length() - 1);
    desc.erase(0, desc.find_first_not_of(' '));

    return desc;
}

inline void trimKeyword(std::string &instruction, const std::string keyword) {
    // Trim a keyword from an instruction.
    // A helper function for parsing.
    instruction.erase(0, keyword.length());
    instruction.erase(0, instruction.find_first_not_of(' '));
}
std::vector<std::string> *parseInstruction(Instruction instruction) {
    // Return a vector of tokens to make executing instructions simpler.

    // parsedInstruction dynamically allocated so that nullptr can be returned
    // if the intruction is an argument or description.
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
        delete parsedInstruction;
        return nullptr;
    }

    return parsedInstruction;
}

template <typename T>
void freeDocList(std::vector<T*> vec) {
    // For freeing vectors of dynamically allocated
    // FuncDoc and ClassDoc objects in a vector.
    for (T* element : vec) {
        delete element;
    }
}

void genDoc_file(const std::string &fileName) {
    // Concatenate the function/class docs in a file to buffers.
    try {
        if (getFileType(fileName) == LANG::OTHER)
            throw unsupported_filetype_error(fileName);
        else if (getFileType(fileName) == LANG::PYTHON) {
            std::ifstream file(fileName);
            std::ofstream docFile;
            docFile.open("documentation.md", std::ios::out | std::ios::app);

            std::string line;

            Instruction instruction;
            std::vector<std::string> *parsedInstruction;

            ClassDoc *currClass = nullptr;
            FuncDoc  *currFunc = nullptr;

            bool isArg = false, isDesc = false;

            std::string prevLine = "";
            for (int lineNo = 1; std::getline(file, line); ++lineNo) {
                // Use a for loop to keep track of line numbers.
                line.erase(0, line.find_first_not_of(' ')); // Trim leading whitespace.
                if (line.substr(0, 3) == "#dg") {
                    line = line.substr(4, line.length());
                    instruction = Instruction(fileName, lineNo, line);

                    parsedInstruction = parseInstruction(instruction);
                    
                    if (parsedInstruction != nullptr) {
                        if ((*parsedInstruction)[0] == "START") {
                            if ((*parsedInstruction)[1] == "CLASS") {
                                if (currFunc) {
                                    throw dg_syntax_error("Cannot document a CLASS inside a FUNC", fileName, lineNo);
                                } else if (currClass) {
                                    throw dg_syntax_error("Cannot document a CLASS inside a CLASS", fileName, lineNo);
                                }
                                currClass = new ClassDoc();

                                prevLine.erase(0, prevLine.find_first_not_of(' '));
                                prevLine.erase(prevLine.find_last_not_of(' ') + 1);
                                prevLine.insert(6, "__");

                                currClass->setName(prevLine.substr(0, prevLine.length() - 1) + "__");

                                docFile << currClass->name() << "\n";
                            } else if ((*parsedInstruction)[1] == "FUNC") {
                                if (currFunc) {
                                    throw dg_syntax_error("Cannot document a FUNC inside a FUNC", fileName, lineNo);
                                }

                                if (currClass) {
                                    currFunc = new FuncDoc(currClass);
                                } else {
                                    currFunc = new FuncDoc();
                                }

                                prevLine.erase(0, prevLine.find_first_not_of(' '));
                                prevLine.erase(prevLine.find_last_not_of(' ') + 1);
                                prevLine.erase(0, 4);

                                for (size_t i = 0; i < prevLine.length(); ++i) {
                                    // Escape underscores in function names because
                                    // underscores are part of markdown notation.
                                    if (prevLine[i] == '_') {
                                        prevLine.insert(i, "\\");
                                        ++i;
                                    }
                                }
                                
                                std::string name = std::string("function __") + prevLine.substr(0, prevLine.length() - 1);
                                name.insert(name.find_first_of('('), "__");

                                currFunc->setName(name);
                            } else if ((*parsedInstruction)[1] == "ARGS") {
                                isArg = true;
                            } else if ((*parsedInstruction)[1] == "DESC") {
                                isDesc = true;
                            } else {
                                throw dg_syntax_error("Invalid START type \"" + (*parsedInstruction)[1] + "\"", fileName, lineNo);
                            }
                        } else if ((*parsedInstruction)[0] == "END") {
                            if ((*parsedInstruction)[1] == "CLASS") {
                                delete currClass;
                                currClass = nullptr;
                            } else if ((*parsedInstruction)[1] == "FUNC") {
                                docFile << currFunc->name() << "\n"
                                        << currFunc->args() << "\n"
                                        << currFunc->returnType() << "\n"
                                        << currFunc->desc() << "\n";

                                delete currFunc;
                                currFunc = nullptr;
                            } else if ((*parsedInstruction)[1] == "ARGS") {
                                isArg = false;
                            } else if ((*parsedInstruction)[1] == "DESC") {
                                if (currClass && !currFunc) {
                                    docFile << currClass->desc() << "\n";
                                }
                                isDesc = false;
                            } else {
                                throw dg_syntax_error("Invalid END type \"" + (*parsedInstruction)[1] + "\"", fileName, lineNo);
                            }
                        } else if ((*parsedInstruction)[0] == "RETURNS") {
                            if (!currFunc) {
                                throw dg_syntax_error("Cannot use RETURNS outside of a FUNC", fileName, lineNo);
                            }
                            std::string returnTypes = (*parsedInstruction)[1];
                            for (size_t i = 2; i < parsedInstruction->size(); ++i) {
                                returnTypes += std::string("\n    ") + (*parsedInstruction)[i];
                            }
                            currFunc->setReturnType(returnTypes);
                        } else if ((*parsedInstruction)[0] == "BRIEF") {
                            if (currFunc) {
                                currFunc->addDescLine((*parsedInstruction)[1]);
                            } else if (currClass) {
                                currClass->addDescLine((*parsedInstruction)[1]);
                                docFile << currClass->desc() << "\n";
                            } else {
                                throw dg_syntax_error("Cannot use BRIEF outside of a FUNC or CLASS", fileName, lineNo);
                            }
                        }
                    } else {
                        // If an instruction is not parsed by parseInstruction and we are not
                        // in an argument/description block.
                        throw dg_syntax_error("Invalid instruction", fileName, lineNo);
                    }
                    delete parsedInstruction;
                } else if (isArg) {
                    if (currFunc == nullptr) {
                        throw dg_syntax_error("Args defined while not in function", fileName, lineNo);
                    }
                    currFunc->addArg(parseFuncArg(line));
                } else if (isDesc) {
                    if (currFunc) {
                        currFunc->addDescLine(parseDesc(line));
                    } else if (currClass) {
                        currClass->addDescLine(parseDesc(line));
                    } else {
                        throw dg_syntax_error("Desc defined while not in class or function", fileName, lineNo);
                    }
                } 
                prevLine = line;
            }
        file.close();
        docFile.close();
        }
    } catch (const unsupported_filetype_error &err) {
        std::cerr << err.what() << std::endl;
    } catch (const dg_syntax_error &err) {
        std::cerr << err.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
#ifndef DGCLASSES_HPP
#define DGCLASSES_HPP

#include <iostream>
#include <string>
#include <vector>

/*
Docgen Keywords:
    START,
    ARGS_START,
    ARGS_END,
    RETURNS,
    DESC_BEG,
    DESC_END,
    BRIEF

    FUNC,
    MEM_FUNC,
    MEM_VAR,
    CLASS,
    GLOBAL_VAR
*/

// Enums
struct LANG{
    enum Enum {
        PYTHON,
        OTHER
    };
};

// Classes
struct Arg {
    // A struct that holds all of the info of an arg.
    // A filled version of this struct is returned
    // by parseArg()
    std::string types;
    std::string name;
    std::string desc;
};

struct Instruction {
    // A struct that holds an instruction,
    // the line it was written on, and the
    // file that it is contained in for
    // debugging purposes.
    Instruction(std::string fileName_arg, int lineNo, std::string instruction):
        lineNo(lineNo),
        instruction(instruction) {
            // Get just the file name (trim the path to the file).
            fileName_arg.erase(0, (fileName_arg.find_last_of('/') == fileName_arg.npos) ? 0 : fileName_arg.find_last_of('/') + 1);
            fileName = fileName_arg;
    }
    Instruction() = default;

    int lineNo;
    std::string instruction;
    std::string fileName;
};

class ClassDoc;
class FuncDoc {
    public:
        FuncDoc(ClassDoc *containingClass = nullptr, LANG::Enum lang = LANG::PYTHON):
            lang_(lang),
            args_("####Arguments:"),
            desc_("####Description:"),
            returnType_("####Returns: "),
            class_(containingClass) {
                if (lang == LANG::PYTHON) {
                    name_ = "";
                }

                // Member functions are h3 while normal functions are h2.
                if (!containingClass) {
                    name_ += std::string("##");
                } else {
                    name_ += std::string("###");
                }
        }

        void setName(const std::string name) {
            name_ += name;
        }
        void setReturnType(const std::string returnType) {
            returnType_ = std::string("####Return Type:\n    ") + returnType;
        }
        void addArg(const Arg arg) {
            args_ += std::string("\n    ") + arg.types + " " + arg.name + ": " + arg.desc;
        }
        void addDescLine(const std::string line) {
            desc_ += std::string("\n    ") + line;
        }

        // Getters.
        std::string name() const {
            return name_;
        }
        std::string args() const {
            return args_;
        }
        std::string desc() const {
            return desc_;
        }
        std::string returnType() const {
            return returnType_;
        }
    private:
        LANG::Enum  lang_;
        std::string name_;
        std::string args_;
        std::string desc_;
        std::string returnType_;
        ClassDoc *class_;
};


class ClassDoc {
    public:
        ClassDoc(LANG::Enum lang = LANG::PYTHON):
            lang_(lang),
            name_("##class "),
            desc_("####Description:") {}

        void setName(const std::string name) {
            name_ += name;
        }
        void addDescLine(const std::string line) {
            desc_ += std::string("\n    ") + line;
        }

        std::string name() const {
            return name_;
        }
    private:
        LANG::Enum  lang_;
        std::string name_;
        std::string desc_;
};


#endif
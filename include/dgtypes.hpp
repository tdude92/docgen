#ifndef DGCLASSES_HPP
#define DGCLASSES_HPP

#include <iostream>
#include <string>
#include <vector>

// TODO: Write class for ClassDoc!

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
    CLASS
*/

// Enums
struct LANG{
    enum Enum {
        PYTHON,
        CPP,
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

class FuncDoc {
    public:
        FuncDoc(std::string name, std::string returnType, std::string funcType, LANG::Enum lang):
            lang_(lang),
            args_("####Arguments:"),
            desc_("####Description:") {
                if (lang == LANG::CPP) {
                    name_ = returnType + " ";
                } else if (lang == LANG::PYTHON) {
                    name_ = "";
                }

                // Member functions are h3 while normal functions are h2.
                if (funcType == "FUNC") {
                    name_ += std::string("##") + name + "(";
                } else if (funcType == "MEM_FUNC") {
                    name_ += std::string("###") + name + "(";
                } else {
                    std::cerr << "Invalid type for FuncDoc!" << std::endl;
                }
        }

        void setName(std::string name) {
            name_ += name;
        }
        void setReturnType(std::string returnType) {
            returnType_ = std::string("####Return Type:\n    ") + returnType;
        }
        void addArg(Arg arg) {
            args_ += std::string("\n    ") + arg.types + " " + arg.name + ": " + arg.desc;
        }
        void addDescLine(std::string line) {
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
};

#endif
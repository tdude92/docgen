#ifndef DGEXCEPT_HPP
#define DGEXCEPT_HPP

#include <exception>
#include <string>
#include <iostream>


class unsupported_filetype_error: public std::exception {
    public:
        unsupported_filetype_error(std::string file): file_(file) {
            file_.erase(0, (file_.find_last_of('/') == file_.npos) ? 0 : file_.find_last_of('/') + 1);
        }
        
        std::string errmsg() const {
            return (std::string("\"") + file_ +  "\" is of an unsupported filetype. Skipping...").c_str();
        }
    private:
        std::string file_;
};


class dg_syntax_error: public std::exception {
    public:
        dg_syntax_error(std::string desc, std::string file, int line):
            desc_(desc),
            file_(file),
            line_(line) {
                file_.erase(0, (file_.find_last_of('/') == file_.npos) ? 0 : file_.find_last_of('/') + 1);
        }

        std::string errmsg() const {
            return ("(" + file_ + ") " + "Syntax error on line " + std::to_string(line_) + ": " + desc_ + ". Stop.").c_str();
        }
    private:
        std::string desc_;
        std::string file_;
        int line_;
};

#endif
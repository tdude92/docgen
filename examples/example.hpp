#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <string>
#include <iostream>

const std::string randomFluff;
const int a;

std::string string;

void foo();
double bar(double a, double b);

class foobar {
    public:
        foobar(float dollars, int age): dollars(dollars), age(age) {

        }
    private:
        void sayHi() {
            std::cout << "Hi, i'm foobar" << std::endl;
        }

        float dollars;
        int age;
};

#endif
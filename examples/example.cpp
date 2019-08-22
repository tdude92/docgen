#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>

#include "example.hpp"

const std::string randomFluff = "for testing";
const int a = 20;

void foo() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Hello, world!" << std::endl;
}

double bar(double a, double b) {
    srand(time(NULL));
    return a + b + rand() % 10;
}

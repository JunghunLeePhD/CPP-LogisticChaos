#include <iostream>
#include <vector>
#include "logistic.h"
#include "composition.h"

int main() {
    std::function<double(double)> f = [](double x) { return 2.0 * x; };

    auto f0 = power_func(f, 0);
    std::cout << "f^0(5) = " << f0(5) << std::endl; // Output: 5

    auto f3 = power_func(f, 3);
    std::cout << "f^3(5) = " << f3(5) << std::endl; // Output: 40

    return 0;
}
#include <iostream>
#include <vector>
#include "logistic.h"
#include "composition.h"

int main() {
    std::vector<double> result = get_orbit(3.9, 0.5, 100, 10);

    std::cout << "Orbit values: ";
    for (double val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
#include "logistic.h"
#include <algorithm>
#include <iterator>

std::vector<double> get_orbit(double r, double x0, int iterations) {
    std::vector<double> orbit(iterations + 1);

    orbit[0] = x0;
    double current_x = x0;

    auto logistic_map = make_logistic_map(r);

    std::generate(std::next(orbit.begin()), orbit.end(), [=]() mutable {
        current_x = logistic_map(current_x); 
        return current_x;
    });

    return orbit;
}
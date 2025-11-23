#include "logistic.h"
#include "composition.h" 
#include <algorithm>
#include <iterator>

std::vector<double> get_orbit(double r, double x0, int iterations, int skip_transient) {
    std::vector<double> orbit(iterations + 1);
    std::function<double(double)> map_func = make_logistic_map(r);

    double start_x;

    if (skip_transient > 0) {
        auto jump_func = power_func(map_func, skip_transient);
        start_x = jump_func(x0); 
    } else {
        start_x = x0;
    }

    orbit[0] = start_x;
    double current_x = start_x;

    std::generate(std::next(orbit.begin()), orbit.end(), [=]() mutable {
        current_x = map_func(current_x);
        return current_x;
    });

    return orbit;
}
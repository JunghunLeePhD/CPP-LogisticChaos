#include "composition.h"
#include <algorithm>

std::function<double(double)> identity_func = [](double x) { 
    return x; 
};

std::function<double(double)> power_func(std::function<double(double)> f, int n) {
    if (n <= 0) {
        return identity_func; 
    }
    if (n % 2 == 0) {
        auto g = power_func(f, n / 2);
        return compose(g, g);
    } else {
        auto g = power_func(f, n - 1);
        return compose(f, g);
    }
}

std::vector<double> get_orbit(std::function<double(double)> f, double x0, int iterations, int skip_transient) {
    std::vector<double> orbit(iterations + 1);

    double start_x;
    auto jump_func = power_func(f, skip_transient);
    start_x = jump_func(x0);

    orbit[0] = start_x;
    double current_x = start_x;

    std::generate(std::next(orbit.begin()), orbit.end(), [&]() mutable {
        current_x = f(current_x);
        return current_x;
    });

    return orbit;
}
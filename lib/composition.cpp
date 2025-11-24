#include "composition.h"
#include <algorithm> 
#include <iterator>  
#include <vector>

std::function<double(double)> identity_func = [](double x) { 
    return x; 
};

std::function<double(double)> power_func(std::function<double(double)> f, int n) {
    if (n <= 0) {
        return identity_func;
    }

    return [f, n](double x) {
        double result = x;
        for (int i = 0; i < n; ++i) {
            result = f(result);
        }
        return result;
    };
}

std::vector<double> get_orbit(std::function<double(double)> f, double x0, int iterations, int skip_transient) {
    std::vector<double> orbit(iterations + 1);

    double current_x = power_func(f, skip_transient)(x0);

    orbit[0] = current_x;

    std::generate(std::next(orbit.begin()), orbit.end(), [&current_x, &f]() {
        current_x = f(current_x);
        return current_x;
    });

    return orbit;
}
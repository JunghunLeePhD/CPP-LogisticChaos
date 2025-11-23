#ifndef COMPOSITION_H
#define COMPOSITION_H

<<<<<<< HEAD
#include <functional>
#include <vector>

extern std::function<double(double)> identity_func;
=======
#include <iostream>
#include <functional>

auto identity_func = [](double x) { 
    return x; 
};
>>>>>>> de870e0 (Update (`composition.h`): add `identity_func`, `compose`, `pow_func` functions)

template<typename F, typename G>
std::function<double(double)> compose(F f, G g) {
    return [f, g](double x) {
        return f(g(x));
    };
}

<<<<<<< HEAD
std::function<double(double)> power_func(std::function<double(double)> f, int n);
=======
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
>>>>>>> de870e0 (Update (`composition.h`): add `identity_func`, `compose`, `pow_func` functions)

std::vector<double> get_orbit(std::function<double(double)> f, double x0, int iterations, int skip_transient = 0);

#endif
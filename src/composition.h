#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <iostream>
#include <functional>

auto identity_func = [](double x) { 
    return x; 
};

template<typename F, typename G>
std::function<double(double)> compose(F f, G g) {
    return [f, g](double x) {
        return f(g(x));
    };
}

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

#endif
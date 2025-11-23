#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <functional>

extern std::function<double(double)> identity_func;

template<typename F, typename G>
std::function<double(double)> compose(F f, G g) {
    return [f, g](double x) {
        return f(g(x));
    };
}

std::function<double(double)> power_func(std::function<double(double)> f, int n);

#endif
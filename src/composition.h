#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <functional>
#include <vector>

extern std::function<double(double)> identity_func;

template<typename F, typename G>
std::function<double(double)> compose(F f, G g) {
    return [f, g](double x) {
        return f(g(x));
    };
}

std::function<double(double)> power_func(std::function<double(double)> f, int n);

std::vector<double> get_orbit(std::function<double(double)> f, double x0, int iterations, int skip_transient = 0);

#endif
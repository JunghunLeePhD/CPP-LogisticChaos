#include "composition.h"

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
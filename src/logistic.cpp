#include "logistic.h"
#include <algorithm>
#include <iterator> 

double logisticMap(double r, double x){
    return r * x * (1.0 - x);
    
}

std::vector<double> get_orbit(double r, double x0, int iterations) {
    std::vector<double> orbit(iterations + 1);

    orbit[0] = x0;
    double current_x = x0; 

    std::generate(std::next(orbit.begin()), orbit.end(), [=]() mutable {
        current_x = logisticMap(r, current_x);
        return current_x;
    });

    return orbit;
}
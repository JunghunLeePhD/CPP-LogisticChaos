#ifndef LOGISTIC_H
#define LOGISTIC_H

#include <vector>

inline auto make_logistic_map(double r) {
    return [r](double x) { 
        return r * x * (1.0 - x); 
    };
}

#endif
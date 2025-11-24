#ifndef LOGISTIC_H
#define LOGISTIC_H

#include <algorithm> 
#include <cmath>     

struct LogisticMap {
    double r;

    [[nodiscard]] double operator()(double x) const {
        return r * x * (1.0 - x);
    }
};

[[nodiscard]] inline auto make_logistic_map(double r) {
    return LogisticMap{r};
}

#endif
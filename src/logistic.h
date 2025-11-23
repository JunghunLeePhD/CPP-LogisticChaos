#ifndef LOGISTIC_H
#define LOGISTIC_H

#include <vector> 

double logisticMap(double r, double x);

std::vector<double> get_orbit(double r, double x0, int iterations);

#endif
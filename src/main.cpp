#include <iostream>
#include <vector>
#include <iomanip>
#include "logistic.h"
// Include the new header
#include "plotter.h" 

int main() {
    double r_chaos = 3.9;
    double r_stable = 2.9;
    double x0 = 0.5;
    int iterations = 50;

    int skip = 0; 

    std::cout << "Generating chaotic orbit (r=" << r_chaos << ")..." << std::endl;
    std::vector<double> chaos_orbit = get_orbit(r_chaos, x0, iterations, skip);

    save_orbit_image("chaos_orbit.ppm", chaos_orbit, 1000, 4);

    std::cout << "Done. Check the folder." << std::endl;

    return 0;
}
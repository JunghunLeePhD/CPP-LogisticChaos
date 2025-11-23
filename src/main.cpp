#include <iostream>
#include <vector>
#include "logistic.h"
#include "plotter.h"

int main() {
    double r = 3.9; 
    double x0 = 0.5;
    int iterations = 100; 

    std::cout << "Generating orbit..." << std::endl;
    std::vector<double> orbit = get_orbit(r, x0, iterations);

    save_orbit_image("output/orbit.ppm", orbit, 1000, 100);
    save_cobweb_plot("output/cobweb.ppm", orbit, r, 800, 800);

    std::cout << "Done!" << std::endl;
    return 0;
}
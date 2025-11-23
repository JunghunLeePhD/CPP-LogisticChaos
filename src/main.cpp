#include <iostream>
#include <vector>
#include <string> 
#include "logistic.h"
#include "plotter.h"

int main() {
    double r = 3.9;       
    double x0 = 0.5;      
    int iterations = 100; 

    std::cout << "Calculating orbit..." << std::endl;
    std::vector<double> orbit = get_orbit(r, x0, iterations);

    system("mkdir -p output/cobweb_frames");

    save_cobweb_animation("output/cobweb_frames", orbit, r, 800, 800);

    std::cout << "\nTo create the video, run:\n";
    std::cout << "ffmpeg -framerate 10 -i output/cobweb_frames/cobweb_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/cobweb.mp4\n";

    return 0;
}
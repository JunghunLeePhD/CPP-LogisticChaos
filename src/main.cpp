#include <iostream>
#include <vector>
#include <string>
#include "logistic.h"
#include "plotter.h"

int main() {
    double r = 3.9;
    double x0 = 0.5;
    int iterations = 200; 
    
    std::cout << "Calculating orbit..." << std::endl;
    std::vector<double> orbit = get_orbit(r, x0, iterations);

    system("mkdir -p output/frames");
    save_orbit_animation("output/frames", orbit, 800, 50);

    std::cout << "\nTo make a video, install ffmpeg and run:" << std::endl;
    std::cout << "ffmpeg -framerate 30 -i output/frames/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/orbit_video.mp4" << std::endl;

    return 0;
}
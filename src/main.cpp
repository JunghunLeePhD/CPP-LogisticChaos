#include <iostream>
#include <vector>
#include <string>
#include "logistic.h"
#include "plotter.h"

int main() {
    double r = 3.9;
    auto orbit = get_orbit(r, 0.3, 100);
    
    Color black(0,0,0);
    Color gray(100,100,100);
    Color white(255,255,255);
    Color blue(170,220,255);
    Color gold(255,215,0);

    system("mkdir -p output/frames_strip");

    PlotCanvas(800, 100)
        .fill_background(black)
        .draw_baseline(50, gray)
        .animate_strip("output/frames_strip", orbit, blue, gold);

    auto map_func = make_logistic_map(r);

    system("mkdir -p output/frames_cobweb");
    PlotCanvas(800, 800)
        .fill_background(black)
        .draw_diagonal(gray)
        .draw_function(map_func, white)
        .animate_cobweb("output/frames_cobweb", orbit, blue, gold);

    std::cout << "All tasks completed." << std::endl;

    std::cout << "\nTo create the video, run:\n";
    std::cout << "ffmpeg -framerate 10 -i output/frames_cobweb/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/cobweb.mp4\n";
    std::cout << "ffmpeg -framerate 10 -i output/frames_strip/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/strip.mp4\n";
    return 0;
}
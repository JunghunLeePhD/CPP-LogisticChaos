#include <iostream>
#include <vector>
#include <string>
#include "composition.h"
#include "logistic.h"
#include "plotter.h"
#include "animator.h" 

int main() {
    Color black(0,0,0);
    Color gray(100,100,100);
    Color white(255,255,255);
    Color blue(170,220,255);
    Color gold(255,215,0);

    auto logistic = make_logistic_map(3.9);
    auto orbit = get_orbit(logistic, 0.5, 100);
    
    system("mkdir -p output/frames_strip");

    PlotCanvas(800, 100)
        .fill_background(black)
        .draw_baseline(50, gray)
        .animate_strip("output/frames_strip", orbit, blue, gold);


    system("mkdir -p output/frames_cobweb");
    PlotCanvas(800, 800)
        .fill_background(black)
        .draw_diagonal(gray)
        .draw_function(logistic, white)
        .animate_cobweb("output/frames_cobweb", orbit, blue, gold);
    
    int ITERATIONS = 300;
    OrbitAnimator(800, 750)
        .set_background_color(black)
        .set_diagonal_color(gray)
        .set_function_color(white)
        .set_web_colors(blue, gold)
        .set_map_factory(make_logistic_map)
        .set_start_x(0.5)
        .set_iterations(ITERATIONS)
        .run_cobweb_sweep(1.0, 4.0, 1200, "output/frames_cobweb_sweep");

    OrbitAnimator(800, 50)
        .set_background_color(black)
        .set_diagonal_color(gray)
        .set_web_colors(blue, gold)
        .set_map_factory(make_logistic_map)
        .set_start_x(0.5)
        .set_iterations(ITERATIONS)
        .run_strip_sweep(1.0, 4.0, 1200, "output/frames_strip_sweep");;

    OrbitAnimator(800, 800)
        .set_background_color(black)
        .set_web_colors(blue, gold) 
        .set_map_factory(make_logistic_map)
        .set_start_x(0.5)      
        .set_iterations(ITERATIONS)   
        .run_bifurcation_growth(1.0, 4.0, 1200, "output/frames_bifurcation_growth");

    OrbitAnimator(800, 800)
        .set_background_color(black)
        .set_web_colors(blue, gold)
        .set_map_factory(make_logistic_map)
        .set_iterations(ITERATIONS)
        .run_cobweb_sweep_initial_point(3.2, 1200, "output/frame_cobweb_sweep_initial_point");


    std::cout << "All tasks completed. \nTo create the video, run:" << std::endl;
    std::cout << "ffmpeg -framerate 10 -i output/frames_cobweb/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/cobweb.mp4" << std::endl;
    std::cout << "ffmpeg -framerate 10 -i output/frames_strip/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/strip.mp4" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i output/frames_cobweb_sweep/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/cobweb_sweep.mp4" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i output/frames_strip_sweep/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/strip_sweep.mp4" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i output/frames_bifurcation_growth/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/bifurcation_growth.mp4" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i output/frame_cobweb_sweep_initial_point/frame_%04d.ppm -c:v libx264 -pix_fmt yuv420p output/cobweb_sweep_initial_point.mp4" << std::endl;

    return 0;
}
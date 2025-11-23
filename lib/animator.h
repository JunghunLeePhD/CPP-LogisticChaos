#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "plotter.h"
#include <string>
#include <functional>

using MapFactory = std::function<std::function<double(double)>(double)>;

class OrbitAnimator {
private:
    int width;
    int height;
    int iterations = 100;
    double x0 = 0.1;

    MapFactory mapFactory = [](double r) { 
        return [r](double x) { return r * x * (1.0 - x); };
    };    

    Color bgColor = Color(0,0,0);
    Color diagColor = Color(100,100,100);
    Color funcColor = Color(255, 255, 255);
    Color webStart = Color(170,220,255);
    Color webEnd = Color(255,215,0);
    
public:
    OrbitAnimator(int w, int h);

    OrbitAnimator& set_iterations(int iter);
    OrbitAnimator& set_start_x(double x);
    OrbitAnimator& set_map_factory(MapFactory factory);

    OrbitAnimator& set_background_color(const Color& c);
    OrbitAnimator& set_diagonal_color(const Color& c);
    OrbitAnimator& set_function_color(const Color& c);
    OrbitAnimator& set_web_colors(const Color& start, const Color& end);

    void run_cobweb_sweep(double r_start, double r_end, int total_frames, const std::string& output_folder);

    void run_strip_sweep(double r_start, double r_end, int total_frames, const std::string& output_folder);

    void run_bifurcation_growth(double r_start, double r_end, int total_frames, const std::string& output_folder);
};

#endif
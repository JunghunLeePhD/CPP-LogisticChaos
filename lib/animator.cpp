#include "animator.h"
#include "composition.h" 
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

OrbitAnimator::OrbitAnimator(int w, int h) : width(w), height(h) {}

OrbitAnimator& OrbitAnimator::set_iterations(int iter) {
    iterations = iter;
    return *this;
}

OrbitAnimator& OrbitAnimator::set_start_x(double x) {
    x0 = x;
    return *this;
}

OrbitAnimator& OrbitAnimator::set_map_factory(MapFactory factory) {
    mapFactory = factory;
    return *this;
}

OrbitAnimator& OrbitAnimator::set_background_color(const Color& c) {
    bgColor = c; return *this;
}

OrbitAnimator& OrbitAnimator::set_diagonal_color(const Color& c) {
    diagColor = c; return *this;
}

OrbitAnimator& OrbitAnimator::set_function_color(const Color& c) {
    funcColor = c; return *this;
}

OrbitAnimator& OrbitAnimator::set_web_colors(const Color& start, const Color& end) {
    webStart = start; webEnd = end; return *this;
}

void OrbitAnimator::run_cobweb_sweep(double r_start, double r_end, int total_frames, const std::string& output_folder) {
    std::cout << "Starting Sweep Animation in: " << output_folder << std::endl;
    std::string cmd = "mkdir -p " + output_folder;
    system(cmd.c_str());

    for (int i = 0; i < total_frames; ++i) {
        double t = static_cast<double>(i) / (total_frames - 1);
        double r = r_start + t * (r_end - r_start);

        auto map_func = mapFactory(r);
        auto orbit = get_orbit(map_func, x0, iterations);

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";

        PlotCanvas(width, height)
            .fill_background(bgColor)
            .draw_diagonal(diagColor)
            .draw_function(map_func, funcColor) 
            .trace_cobweb(orbit, webStart, webEnd) 
            .save(ss.str());

        if (i % 10 == 0) {
            int pct = (i * 100) / total_frames;
            std::cout << "Rendering: " << pct << "% (r=" << r << ")\r" << std::flush;
        }
    }
    std::cout << "\nAnimation Complete." << std::endl;
}

void OrbitAnimator::run_strip_sweep(double r_start, double r_end, int total_frames, const std::string& output_folder) {
    std::cout << "Starting Strip Sweep Animation in: " << output_folder << std::endl;
    std::string cmd = "mkdir -p " + output_folder;
    system(cmd.c_str());

    for (int i = 0; i < total_frames; ++i) {
        double t = static_cast<double>(i) / (total_frames - 1);
        double r = r_start + t * (r_end - r_start);

        // 1. Generate Logic
        auto map_func = mapFactory(r);
        auto orbit = get_orbit(map_func, x0, iterations);

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";

        // 2. Visualization
        // We reuse 'diagColor' for the baseline color to keep configuration simple
        PlotCanvas(width, height)
            .fill_background(bgColor)
            .draw_baseline(height / 2, diagColor) 
            .draw_strip(orbit, webStart, webEnd)
            .save(ss.str());

        if (i % 10 == 0) {
            int pct = (i * 100) / total_frames;
            std::cout << "Rendering: " << pct << "% (r=" << r << ")\r" << std::flush;
        }
    }
    std::cout << "\nStrip Animation Complete." << std::endl;
}
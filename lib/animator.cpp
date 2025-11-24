#include "animator.h"
#include "composition.h" 
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm> 
#include <numeric>   
#include <execution> 
#include <filesystem>
#include <memory>    

namespace fs = std::filesystem;

template <typename T>
T lerp(T a, T b, double t) {
    return a + t * (b - a);
}

OrbitAnimator::OrbitAnimator(int w, int h) : width(w), height(h) {}

OrbitAnimator& OrbitAnimator::set_iterations(int iter) { iterations = iter; return *this; }
OrbitAnimator& OrbitAnimator::set_start_x(double x)    { x0 = x; return *this; }
OrbitAnimator& OrbitAnimator::set_start_r(double r)    { r0 = r; return *this; }
OrbitAnimator& OrbitAnimator::set_map_factory(MapFactory factory) { mapFactory = factory; return *this; }
OrbitAnimator& OrbitAnimator::set_background_color(const Color& c) { bgColor = c; return *this; }
OrbitAnimator& OrbitAnimator::set_diagonal_color(const Color& c)   { diagColor = c; return *this; }
OrbitAnimator& OrbitAnimator::set_function_color(const Color& c)   { funcColor = c; return *this; }
OrbitAnimator& OrbitAnimator::set_web_colors(const Color& start, const Color& end) { 
    webStart = start; webEnd = end; return *this; 
}

std::vector<int> get_frame_indices(int total) {
    std::vector<int> frames(total);
    std::iota(frames.begin(), frames.end(), 0); // Fills 0, 1, 2... total-1
    return frames;
}

void prepare_directory(const std::string& path) {
    std::cout << "Output directory: " << path << std::endl;
    fs::create_directories(path);
}

void OrbitAnimator::run_cobweb_sweep_parameter(double r_start, double r_end, int total_frames, const std::string& output_folder) {
    prepare_directory(output_folder);
    auto frames = get_frame_indices(total_frames);

    std::for_each(std::execution::par, frames.begin(), frames.end(), [&](int i) {
        double t = static_cast<double>(i) / std::max(1, total_frames - 1);
        double r = lerp(r_start, r_end, t);

        auto map_func = mapFactory(r);
        auto orbit = get_orbit(map_func, x0, iterations);

        auto canvas = std::make_unique<PlotCanvas>(width, height);
        
        canvas->fill_background(bgColor)
              .draw_diagonal(diagColor)
              .draw_function(map_func, funcColor) 
              .trace_cobweb(orbit, webStart, webEnd);

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";
        canvas->save(ss.str()); // Save and automatically destroy canvas via unique_ptr scope

        if (i % 10 == 0) {
            std::cout << "Rendering frame " << i << " (r=" << r << ")\r" << std::flush;
        }
    });

    std::cout << "\nSweep Animation Complete." << std::endl;
}

void OrbitAnimator::run_cobweb_sweep_initial_point(double x_start, double x_end, int total_frames, const std::string& output_folder) {
    prepare_directory(output_folder);
    auto frames = get_frame_indices(total_frames);
    
    auto map_func = mapFactory(r0);

    std::for_each(std::execution::par, frames.begin(), frames.end(), [&](int i) {
        double t = static_cast<double>(i) / std::max(1, total_frames - 1);
        double x = lerp(x_start, x_end, t);

        auto orbit = get_orbit(map_func, x, iterations);

        auto canvas = std::make_unique<PlotCanvas>(width, height);
        canvas->fill_background(bgColor)
              .draw_diagonal(diagColor)
              .draw_function(map_func, funcColor)
              .trace_cobweb(orbit, webStart, webEnd);

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";
        canvas->save(ss.str());
        
        if (i % 10 == 0) std::cout << "Rendering frame " << i << "\r" << std::flush;
    });
    std::cout << "\nInitial Point Sweep Complete." << std::endl;
}

void OrbitAnimator::run_strip_sweep_parameter(double r_start, double r_end, int total_frames, const std::string& output_folder) {
    prepare_directory(output_folder);
    auto frames = get_frame_indices(total_frames);

    std::for_each(std::execution::par, frames.begin(), frames.end(), [&](int i) {
        double t = static_cast<double>(i) / std::max(1, total_frames - 1);
        double r = lerp(r_start, r_end, t);

        auto map_func = mapFactory(r);
        auto orbit = get_orbit(map_func, x0, iterations);

        auto canvas = std::make_unique<PlotCanvas>(width, height);
        canvas->fill_background(bgColor)
              .draw_baseline(height / 2, diagColor) 
              .draw_strip(orbit, webStart, webEnd);

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";
        canvas->save(ss.str());

        if (i % 10 == 0) std::cout << "Rendering frame " << i << "\r" << std::flush;
    });
    std::cout << "\nStrip Animation Complete." << std::endl;
}

void OrbitAnimator::run_strip_sweep_initial_point(double x_start, double x_end, int total_frames, const std::string& output_folder){
    prepare_directory(output_folder);
    auto frames = get_frame_indices(total_frames);
    auto map_func = mapFactory(r0);

    std::for_each(std::execution::par, frames.begin(), frames.end(), [&](int i) {
        double t = static_cast<double>(i) / std::max(1, total_frames - 1);
        double x = lerp(x_start, x_end, t);

        auto orbit = get_orbit(map_func, x, iterations);

        auto canvas = std::make_unique<PlotCanvas>(width, height);
        canvas->fill_background(bgColor)
              .draw_baseline(height / 2, diagColor) 
              .draw_strip(orbit, webStart, webEnd);

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";
        canvas->save(ss.str());

        if (i % 10 == 0) std::cout << "Rendering frame " << i << "\r" << std::flush;
    });
    std::cout << "\nStrip Initial Point Sweep Complete." << std::endl;
}

void OrbitAnimator::run_bifurcation_growth(double r_start, double r_end, int total_frames, const std::string& output_folder) {
    prepare_directory(output_folder);

    auto canvas = std::make_unique<PlotCanvas>(width, height);
    canvas->fill_background(bgColor);

    int rows_drawn_so_far = 0;

    auto frames = get_frame_indices(total_frames);
    
    std::for_each(frames.begin(), frames.end(), [&](int frame) {
        
        double progress = static_cast<double>(frame + 1) / total_frames;
        int target_row_count = static_cast<int>(progress * height);

        int rows_to_draw = std::clamp(target_row_count, 0, height) - rows_drawn_so_far;

        std::vector<int> new_rows(rows_to_draw);
        std::iota(new_rows.begin(), new_rows.end(), rows_drawn_so_far);

        for(int row_idx : new_rows) {
            double t = static_cast<double>(row_idx) / std::max(1, height - 1);
            double r = lerp(r_start, r_end, t);
            
            auto map_func = mapFactory(r);
            auto orbit = get_orbit(map_func, x0, iterations);

            Color c = Color::lerp(webStart, webEnd, t);
            
            int y_pixel = canvas->to_screen_y(t); 
            canvas->draw_row_at(y_pixel, orbit, c);
        }
        
        rows_drawn_so_far += rows_to_draw;

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << frame << ".ppm";
        canvas->save(ss.str());

        if (frame % 10 == 0) {
            int pct = (frame * 100) / total_frames;
            std::cout << "Rendering: " << pct << "% (frame " << frame << ")\r" << std::flush;
        }
    });

    std::cout << "\nGrowth Animation Complete." << std::endl;
}
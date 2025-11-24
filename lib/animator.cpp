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

OrbitAnimator& OrbitAnimator::set_start_r(double r) {
    r0 = r;
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

void OrbitAnimator::run_cobweb_sweep_parameter(double r_start, double r_end, int total_frames, const std::string& output_folder) {
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

void OrbitAnimator::run_cobweb_sweep_initial_point(double x_start, double x_end, int total_frames, const std::string& output_folder) {
    std::cout << "Starting Initial Point Sweep (r=" << r0 << ") in: " << output_folder << std::endl;
    std::string cmd = "mkdir -p " + output_folder;
    system(cmd.c_str());

    auto map_func = mapFactory(r0);

    for (int i = 0; i < total_frames; ++i) {
        double t = static_cast<double>(i) / (total_frames - 1);
        double x = x_start + t * (x_end - x_start);

        auto orbit = get_orbit(map_func, x, iterations);

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
            std::cout << "Rendering: " << pct << "% (x0=" << x << ")\r" << std::flush;
        }
    }
    std::cout << "\nInitial Point Sweep Complete." << std::endl;
}

void OrbitAnimator::run_strip_sweep_parameter(double r_start, double r_end, int total_frames, const std::string& output_folder) {
    std::cout << "Starting Strip Sweep Animation in: " << output_folder << std::endl;
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

void OrbitAnimator::run_bifurcation_growth(double r_start, double r_end, int total_frames, const std::string& output_folder) {
    std::cout << "Starting Bifurcation Growth in: " << output_folder << std::endl;
    std::string cmd = "mkdir -p " + output_folder;
    system(cmd.c_str());

    PlotCanvas canvas(width, height);
    canvas.fill_background(bgColor);

    int rows_drawn_so_far = 0;

    for (int frame = 0; frame < total_frames; ++frame) {
        
        double progress = static_cast<double>(frame + 1) / total_frames;
        int target_row_count = static_cast<int>(progress * height);

        while (rows_drawn_so_far < target_row_count && rows_drawn_so_far < height) {
            double t = static_cast<double>(rows_drawn_so_far) / (height - 1);
            double r = r_start + t * (r_end - r_start);
            
            auto map_func = mapFactory(r);
            auto orbit = get_orbit(map_func, x0, iterations);

            Color c = Color::lerp(webStart, webEnd, t);
            
            int y_pixel = canvas.to_screen_y(t); 
            
            canvas.draw_row_at(y_pixel, orbit, c);

            rows_drawn_so_far++;
        }

        std::stringstream ss;
        ss << output_folder << "/frame_" << std::setfill('0') << std::setw(4) << frame << ".ppm";
        canvas.save(ss.str());

        if (frame % 10 == 0) {
            int pct = (frame * 100) / total_frames;
            std::cout << "Rendering: " << pct << "% (frame " << frame << ")\r" << std::flush;
        }
    }
    std::cout << "\nGrowth Animation Complete." << std::endl;
}
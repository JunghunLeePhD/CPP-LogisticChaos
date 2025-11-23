#include "plotter.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

PlotCanvas::PlotCanvas(int w, int h) : width(w), height(h) {
    pixels.resize(width * height * 3);
    std::fill(pixels.begin(), pixels.end(), 0); 
}

void PlotCanvas::set_pixel(int x, int y, const Color& c) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int idx = (y * width + x) * 3;
        pixels[idx] = c.r; 
        pixels[idx+1] = c.g; 
        pixels[idx+2] = c.b;
    }
}

int PlotCanvas::to_screen_x(double val) const {
    return static_cast<int>(val * (width - 1));
}

int PlotCanvas::to_screen_y(double val) const {
    return height - 1 - static_cast<int>(val * (height - 1));
}

void PlotCanvas::draw_line_raw(int x0, int y0, int x1, int y1, const Color& c) {
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (true) {
        set_pixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void PlotCanvas::draw_block_raw(int cx, int cy, int w, int h, const Color& c) {
    int halfW = w / 2;
    int halfH = h / 2;
    for (int dx = -halfW; dx <= halfW; ++dx) {
        for (int dy = -halfH; dy <= halfH; ++dy) {
            set_pixel(cx + dx, cy + dy, c);
        }
    }
}

PlotCanvas& PlotCanvas::fill_background(const Color& c) {
    for (size_t i = 0; i < pixels.size(); i += 3) {
        pixels[i] = c.r; 
        pixels[i+1] = c.g; 
        pixels[i+2] = c.b;
    }
    return *this;
}

PlotCanvas& PlotCanvas::draw_diagonal(const Color& c) {
    draw_line_raw(to_screen_x(0), to_screen_y(0), to_screen_x(1), to_screen_y(1), c);
    return *this;
}

PlotCanvas& PlotCanvas::draw_function(std::function<double(double)> func, const Color& c) {
    int prev_px = to_screen_x(0);
    int prev_py = to_screen_y(func(0));
    for (int i = 1; i < width; ++i) {
        double x = static_cast<double>(i) / width;
        double y = func(x);
        if (y > 1.0 || y < 0.0) continue;
        int px = to_screen_x(x);
        int py = to_screen_y(y);
        draw_line_raw(prev_px, prev_py, px, py, c);
        prev_px = px; 
        prev_py = py;
    }
    return *this;
}

PlotCanvas& PlotCanvas::draw_baseline(int y_pos, const Color& c) {
    for(int x=0; x<width; ++x){
        set_pixel(x, y_pos, c);
    }
    return *this;
}

PlotCanvas& PlotCanvas::draw_row_at(int y_pixel, const std::vector<double>& orbit, const Color& c) {
    if (y_pixel < 0 || y_pixel >= height) return *this;

    for (double val : orbit) {
        if (val < 0.0 || val > 1.0) continue;
        
        int x = to_screen_x(val);
        
        if (x >= 0 && x < width) {
            set_pixel(x, y_pixel, c);
        }
    }
    return *this;
}

PlotCanvas& PlotCanvas::draw_strip(const std::vector<double>& orbit, const Color& startC, const Color& endC) {
    int centerY = height / 2;
    size_t total = orbit.size();
    
    for (size_t i = 0; i < total; ++i) {
        if (orbit[i] < 0 || orbit[i] > 1) continue;
        
        double t = (total > 1) ? (double)i / (total - 1) : 0.0;
        Color c = Color::lerp(startC, endC, t);
        
        int tx = to_screen_x(orbit[i]);
        draw_block_raw(tx, centerY, 3, 3, c);
    }
    return *this;
}

PlotCanvas& PlotCanvas::trace_cobweb(const std::vector<double>& orbit, const Color& startC, const Color& endC) {
    if (orbit.empty()) return *this;
    double curr_x = orbit[0];
    size_t total = orbit.size();

    for (size_t i = 0; i < total - 1; ++i) {
        double next_x = orbit[i+1];
        double t = (double)i / (total - 1);
        Color c = Color::lerp(startC, endC, t);

        int ax = to_screen_x(curr_x), ay = to_screen_y(curr_x);
        int bx = to_screen_x(curr_x), by = to_screen_y(next_x);
        int cx = to_screen_x(next_x), cy = to_screen_y(next_x);

        draw_line_raw(ax, ay, bx, by, c);
        draw_line_raw(bx, by, cx, cy, c);
        curr_x = next_x;
    }
    return *this;
}

void PlotCanvas::animate_strip(const std::string& folder, const std::vector<double>& orbit, const Color& startC, const Color& endC) {
    std::cout << "Animating Orbit Strip in " << folder << "..." << std::endl;
    int centerY = height / 2;
    size_t total = orbit.size();

    for (size_t i = 0; i < total; ++i) {
        if (orbit[i] >= 0 && orbit[i] <= 1) {
            double t = (total > 1) ? (double)i / (total - 1) : 0.0;
            Color c = Color::lerp(startC, endC, t);
            
            int tx = to_screen_x(orbit[i]);
            draw_block_raw(tx, centerY, 3, 3, c);
        }

        std::stringstream ss;
        ss << folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";
        save(ss.str());
    }
}

void PlotCanvas::animate_cobweb(const std::string& folder, const std::vector<double>& orbit, const Color& startC, const Color& endC) {
    std::cout << "Animating Cobweb in " << folder << "..." << std::endl;
    if (orbit.empty()) return;
    
    double curr_x = orbit[0];
    size_t total = orbit.size();

    for (size_t i = 0; i < total - 1; ++i) {
        double next_x = orbit[i+1];
        double t = (double)i / (total - 1);
        Color c = Color::lerp(startC, endC, t);

        int ax = to_screen_x(curr_x), ay = to_screen_y(curr_x);
        int bx = to_screen_x(curr_x), by = to_screen_y(next_x);
        int cx = to_screen_x(next_x), cy = to_screen_y(next_x);

        draw_line_raw(ax, ay, bx, by, c);
        draw_line_raw(bx, by, cx, cy, c);
        curr_x = next_x;

        std::stringstream ss;
        ss << folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";
        save(ss.str());
    }
}

void PlotCanvas::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) { std::cerr << "Error opening " << filename << std::endl; return; }
    file << "P6\n" << width << " " << height << "\n255\n";
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
}
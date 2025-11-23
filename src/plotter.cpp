#include "plotter.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip> 
#include <sstream> 

bool save_ppm(const std::string& filename, const std::vector<unsigned char>& pixels, int width, int height) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false;
    }
    
    file << "P6\n" << width << " " << height << "\n255\n";
    
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    file.close();
    
    std::cout << "Image saved to " << filename << std::endl;
    return true;
}

void save_orbit_image(const std::string& filename, const std::vector<double>& orbit, int width, int height) {
    std::vector<unsigned char> pixels(width * height * 3);

    auto set_pixel = [&](int x, int y, const Color& c) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        int index = (y * width + x) * 3;
        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
    };

    std::fill(pixels.begin(), pixels.end(), 0);

    int centerY = height / 2;

    Color gray(150, 150, 150);
    for (int x = 0; x < width; ++x) {
        set_pixel(x, centerY, gray);
    }

    const Color lightBlue(170, 220, 255);
    const Color gold(255, 215, 0);
    
    const double pixel_height = 3.0;
    const double pixel_width  = 3.0;

    size_t total_points = orbit.size();

    for (size_t i = 0; i < total_points; ++i) {
        double val = orbit[i];
        if (val < 0.0 || val > 1.0) continue;

        double t = (total_points > 1) ? static_cast<double>(i) / (total_points - 1) : 0.0;

        Color currentColor = Color::lerp(lightBlue, gold, t);

        int targetX = static_cast<int>(std::round(val * (width - 1)));
        int halfH = static_cast<int>(pixel_height / 2.0);
        int halfW = static_cast<int>(pixel_width / 2.0);

        for (int dx = -halfW; dx <= halfW; ++dx) {
            for (int y = centerY - halfH; y <= centerY + halfH; ++y) {
                set_pixel(targetX + dx, y, currentColor);
            }
        }
    }

    save_ppm(filename, pixels, width, height);
}

void save_orbit_animation(const std::string& folder, const std::vector<double>& orbit, int width, int height) {
    std::vector<unsigned char> pixels(width * height * 3);

    auto set_pixel = [&](int x, int y, const Color& c) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        int index = (y * width + x) * 3;
        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
    };

    std::fill(pixels.begin(), pixels.end(), 0); 

    int centerY = height / 2;
    Color gray(150, 150, 150);
    
    for (int x = 0; x < width; ++x) {
        set_pixel(x, centerY, gray);
    }

    const Color lightBlue(170, 220, 255);
    const Color gold(255, 215, 0);
    const double pixel_height = 3.0;
    const double pixel_width  = 3.0;

    size_t total_points = orbit.size();

    std::cout << "Generating animation frames in '" << folder << "'..." << std::endl;

    for (size_t i = 0; i < total_points; ++i) {
        double val = orbit[i];
        if (val < 0.0 || val > 1.0) continue;

        double t = (total_points > 1) ? static_cast<double>(i) / (total_points - 1) : 0.0;
        
        Color currentColor = Color::lerp(lightBlue, gold, t);

        int targetX = static_cast<int>(std::round(val * (width - 1)));
        int halfH = static_cast<int>(pixel_height / 2.0);
        int halfW = static_cast<int>(pixel_width / 2.0);

        for (int dx = -halfW; dx <= halfW; ++dx) {
            for (int y = centerY - halfH; y <= centerY + halfH; ++y) {
                set_pixel(targetX + dx, y, currentColor);
            }
        }

        std::stringstream ss;
        ss << folder << "/frame_" << std::setfill('0') << std::setw(4) << i << ".ppm";
        
        save_ppm(ss.str(), pixels, width, height);
    }
    
    std::cout << "Animation frames generation complete." << std::endl;
}

void draw_line(std::vector<unsigned char>& pixels, int width, int height, 
               int x0, int y0, int x1, int y1, const Color& c) {
    
    auto set_pixel_unsafe = [&](int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int idx = (y * width + x) * 3;
            pixels[idx] = c.r; pixels[idx+1] = c.g; pixels[idx+2] = c.b;
        }
    };

    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        set_pixel_unsafe(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void save_cobweb_plot(const std::string& filename, const std::vector<double>& orbit, double r, int width, int height) {
    std::vector<unsigned char> pixels(width * height * 3);
    std::fill(pixels.begin(), pixels.end(), 0); // Black background

    auto to_screen_x = [&](double val) { return static_cast<int>(val * (width - 1)); };
    auto to_screen_y = [&](double val) { return height - 1 - static_cast<int>(val * (height - 1)); };

    Color diagColor(100, 100, 100);
    draw_line(pixels, width, height, 
              to_screen_x(0), to_screen_y(0), 
              to_screen_x(1), to_screen_y(1), diagColor);

    Color curveColor(255, 255, 255);
    int prev_px = to_screen_x(0);
    int prev_py = to_screen_y(0);
    
    for (int i = 1; i < width; ++i) {
        double x = static_cast<double>(i) / width;
        double y = r * x * (1.0 - x); 
        
        int px = to_screen_x(x);
        int py = to_screen_y(y);
        
        draw_line(pixels, width, height, prev_px, prev_py, px, py, curveColor);
        prev_px = px; prev_py = py;
    }

    const Color startC(170, 220, 255);
    const Color endC(255, 215, 0);

    double curr_x = orbit[0]; 
    
    for (size_t i = 0; i < orbit.size() - 1; ++i) {
        double next_x = orbit[i+1]; 

        double t = (double)i / (orbit.size() - 1);
        Color c = Color::lerp(startC, endC, t);


        int ax = to_screen_x(curr_x);
        int ay = to_screen_y(curr_x);
        
        int bx = to_screen_x(curr_x);
        int by = to_screen_y(next_x);
        
        int cx = to_screen_x(next_x);
        int cy = to_screen_y(next_x);

        draw_line(pixels, width, height, ax, ay, bx, by, c);
        draw_line(pixels, width, height, bx, by, cx, cy, c);

        curr_x = next_x;
    }

    save_ppm(filename, pixels, width, height);
}
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
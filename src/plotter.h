#ifndef PLOTTER_H
#define PLOTTER_H

#include <vector>
#include <string>
#include <cstdint>

struct Color {
    uint8_t r, g, b;
    Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
    Color() : r(0), g(0), b(0) {}

    static Color lerp(const Color& start, const Color& end, double t) {
        return Color(
            static_cast<uint8_t>(start.r + t * (end.r - start.r)),
            static_cast<uint8_t>(start.g + t * (end.g - start.g)),
            static_cast<uint8_t>(start.b + t * (end.b - start.b))
        );
    }
};

bool save_ppm(const std::string& filename, const std::vector<unsigned char>& pixels, int width, int height);

void save_orbit_image(const std::string& filename, const std::vector<double>& orbit, int width, int height);

#endif
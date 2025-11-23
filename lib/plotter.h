#ifndef PLOTTER_H
#define PLOTTER_H

#include <vector>
#include <string>
#include <functional>
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

class PlotCanvas {
private:
    int width;
    int height;
    std::vector<unsigned char> pixels;

    void set_pixel(int x, int y, const Color& c);
    void draw_line_raw(int x0, int y0, int x1, int y1, const Color& c);
    void draw_block_raw(int x, int y, int w, int h, const Color& c);

    int to_screen_x(double val) const;

public:
    PlotCanvas(int w, int h);

    int to_screen_y(double val) const;

    PlotCanvas& fill_background(const Color& c);
    PlotCanvas& draw_diagonal(const Color& c);
    PlotCanvas& draw_function(std::function<double(double)> func, const Color& c);
    PlotCanvas& draw_baseline(int y_pos, const Color& c);
    PlotCanvas& draw_row_at(int y_pixel, const std::vector<double>& orbit, const Color& c);

    PlotCanvas& draw_strip(const std::vector<double>& orbit, const Color& startC, const Color& endC);

    PlotCanvas& trace_cobweb(const std::vector<double>& orbit, const Color& startC, const Color& endC);

    void animate_strip(const std::string& folder, const std::vector<double>& orbit, const Color& startC, const Color& endC);

    void animate_cobweb(const std::string& folder, const std::vector<double>& orbit, const Color& startC, const Color& endC);

    void save(const std::string& filename);
};

#endif
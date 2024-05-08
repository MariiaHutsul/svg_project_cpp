#include <iostream>

#include "SVGElements.hpp"
#include "PNGImage.cpp"
#include "Point.hpp"

using namespace std;

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Implementation of Ellipse class
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    Color Ellipse::get_color() {
        return fill;
    }
    std::vector<Point> Ellipse::get_points() {
        return {}; // Ellipse does not have points
    }
    Point Ellipse::get_center() {
        return center;
    }
    Point Ellipse::get_radius() {
        return radius;
    }
    const string Ellipse::get_name(){
        return "ellipse";
    }

    // Implementation of Circle class
    Circle::Circle(const Color &fill, const Point &center, int radius)
        : Ellipse(fill, center, {radius, radius}) {}

    // Implementation of Polyline class
    Polyline::Polyline(const std::vector<Point>& points, const Color& stroke)
        : points(points), stroke(stroke) {}

    void Polyline::draw(PNGImage &img) const
    {
        // Draw the line segments of the polyline
        for (size_t i = 0; i < points.size() - 1; ++i) {
            img.draw_line(points[i], points[i + 1], stroke);
        }
    }

    Color Polyline::get_color() {
        return stroke;
    }

    Point Polyline::get_center() {
        //Calculate the centriod of the polyline
        int sum_x = 0, sum_y = 0;
        for (const auto &point : points) {
            sum_x += point.x;
            sum_y += point.y;
        }
        return {sum_x / points.size(), sum_y / points.size()};
    }

    Point Polyline::get_radius(){
         return {0, 0}; // Not applicable for Polyline
    }

    std::vector<Point> Polyline::get_points() {
        return points;
    }

    const string Polyline::get_name() {
        return "polyline";
    }

    Line::Line(const Point &start, const Point &end, const Color &stroke)
        : Polyline({start, end}, stroke) {}

    // Implementation of Polygon class

    Polygon::Polygon(const std::vector<Point>& points, const Color& fill)
        : points(points), fill(fill) {}

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }
    Color Polygon::get_color() {
        return fill;
    }

    Point Polygon::get_center() {
        // Calculate the centroid of the polygon
        int sum_x = 0, sum_y = 0;
        for (const auto &point : points) {
            sum_x += point.x;
            sum_y += point.y;
        }
        return {sum_x / points.size(), sum_y / points.size()};
    }

    Point Polygon::get_radius() {
        return {0, 0}; // Not applicable for Polygon
    }

    std::vector<Point> Polygon::get_points() {
        return points;
    }

    const string Polygon::get_name() {
        return "polygon";
    }

    // Implementation of Rectangle class
    Rectangle::Rectangle(const Point& top_left, int width, int height, const Color& fill)
        : Polygon({top_left, {top_left.x + width, top_left.y},
                   {top_left.x + width, top_left.y + height}, {top_left.x, top_left.y + height}},
                  fill) {}

}
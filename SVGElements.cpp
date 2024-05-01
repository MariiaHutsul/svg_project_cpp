#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
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
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->

    Circle::Circle(const Point& cx, const Point& cy,  const double& r, std::string fill): cx(cx), cy(cy), r(r), fill(fill) {}
    void Circle::draw(PNGImage &img) const
    {
        img.draw_circle(cx, cy, r, fill);
    }


    Polyline::Polyline(const std::vector<Point>& points, const Color& stroke)
        : points(points), stroke(stroke) {}

    void Polyline::draw(PNGImage &img) const
    {
        img.draw_polyline(points, stroke);
    }


    Line::Line(const Point& x1, const Point& y1, const Point& x2, const Point& y2, std::string stroke)
    : x1(x1), y1(y1), x2(x2), y2(y2), stroke(stroke) {}
    void Line::draw(PNGImage &img) const
    {
        img.draw_line(center, radius, fill);
    }


    Polygon::Polygon(const std::vector<Point>& points, const Color& fill)
        : points(points), fill(fill) {}

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }


    Rect::Rect(const Point& x, const Point& y, const double& rx, const double& ry, const double& width, const double& height, std::string stroke)
    : x(x), y(y), rx(rx), ry(ry), width(width), height(height) {}
    void Rect::draw(PNGImage &img) const
    {
        img.draw_rect(x, y, rx, ry, width, height, stroke);
    }

}

// Mariaa i put the code that i did here if u wont u can delete and do it by yourself! (Tiago)
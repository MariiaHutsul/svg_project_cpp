#include "SVGElements.hpp"
#include "PNGImage.cpp"

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
    // Circle

    Circle::Circle(const Point& cx, const Point& cy,  const double& r, std::string fill): cx(cx), cy(cy), r(r), fill(fill) {}
    void Circle::draw(PNGImage &img) const
    {
        // Draw the circle on the PNGImage
        // For simplicity, drawing a circle can be achieved by drawing an ellipse with equal radii
        Point radius_point = {static_cast<int>(r), static_cast<int>(r)};
        img.draw_ellipse(cx, radius_point, parse_color(fill));
    }


    Polyline::Polyline(const std::vector<Point>& points, const Color& stroke)
        : points(points), stroke(stroke) {}

    void Polyline::draw(PNGImage &img) const
    {
        // Draw the polyline on the PNGImage
        for (size_t i = 0; i < points.size() - 1; ++i) {
            img.draw_line(points[i], points[i + 1], stroke);
        }
    }


    Line::Line(const Point& x1, const Point& y1, const Point& x2, const Point& y2, std::string stroke)
    : x1(x1), y1(y1), x2(x2), y2(y2), stroke(std::move(stroke)) {}
    void Line::draw(PNGImage &img) const
    {
        // Draw the line on the PNGImage
        img.draw_line(x1, x2, parse_color(stroke));
    }


    Polygon::Polygon(const std::vector<Point>& points, const Color& fill)
        : points(points), fill(fill) {}

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }


    Rect::Rect(const Point& x, const Point& y, const double& rx, const double& ry, const double& width, const double& height, std::string stroke)
    : x(x), y(y), rx(rx), ry(ry), width(width), height(height), stroke(std::move(stroke)) {}
    void Rect::draw(PNGImage &img) const {
        // Draw the rectangle on the PNGImage
        // For simplicity, drawing a rectangle can be achieved by drawing a polygon with four points
        std::vector<Point> rect_points = {x, {x.x + static_cast<int>(width), x.y},
                                          {x.x + static_cast<int>(width), x.y + static_cast<int>(height)},
                                          {x.x, x.y + static_cast<int>(height)}};
        img.draw_polygon(rect_points, parse_color(stroke));
    }

}
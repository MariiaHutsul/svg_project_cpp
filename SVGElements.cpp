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
    Ellipse::~Ellipse() {};
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->

    Polyline::Polyline(const std::vector<Point>& points, const Color& stroke)
        : points(points), stroke(stroke) {}
    Polyline::~Polyline() {};

    void Polyline::draw(PNGImage &img) const
    {   
        for (size_t i = 0; i < points.size() -1; ++i){
            img.draw_line(points[i], points [i+1], stroke);
        }

    }


    Polygon::Polygon(const std::vector<Point>& points, const Color& fill)
        : points(points), fill(fill) {}
    Polygon::~Polygon() {};
    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }
}
#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse class constructor
    // Initializes with fill color, center point, and radius point
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    // Ellipse class destructor
    Ellipse::~Ellipse() {};

    // Draws the ellipse on the given PNGImage
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->


    // Polyline class constructor
    // Initializes with a vector of points and a stroke color
    Polyline::Polyline(const std::vector<Point>& points, const Color& stroke)
        : points(points), stroke(stroke) {}

    // Polyline class destructor
    Polyline::~Polyline() {};

    // Draws the polyline on the given PNGImage
    // Draws lines between consecutive points
    void Polyline::draw(PNGImage &img) const
    {   
        for (size_t i = 0; i < points.size() -1; ++i){
            img.draw_line(points[i], points [i+1], stroke);
        }

    }

    // Polygon class constructor
    // Initializes with a vector of points and a fill color
    Polygon::Polygon(const std::vector<Point>& points, const Color& fill)
        : points(points), fill(fill) {}
    
    // Polygon class destructor
    Polygon::~Polygon() {};

    // Draws the polygon on the given PNGImage
    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }
}

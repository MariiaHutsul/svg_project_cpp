//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

#include <iostream>  // New
using namespace std; // New

namespace svg {
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);
    
    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };

    class Circle: public SVGElement 
    {
    public:
        Circle(const Point& cx, const Point& cy, const double& r, std::string fill);
        void draw(PNGImage &img) const override;

    private:
        Point cx,cy;
        double r;
        std::string fill;
    };


    class Polyline : public SVGElement
    {
    public:
        Polyline(const std::vector<Point>& points, const Color& stroke);
        void draw(PNGImage &img) const override;

    private:
        std::vector<Point> points;
        Color stroke;
    };

    class Line : public SVGElement
    {
    public:
        Line(const Point& x1, const Point& y1, const Point& x2, const Point& y2, std::string stroke);
        void draw(PNGImage &img) const override;
    private:
        Point x1,x2,y1,y2;
        std::string stroke;
    };


    class Polygon : public SVGElement
    {
    public:
        Polygon(const std::vector<Point>& points, const Color& fill);
        void draw(PNGImage &img) const override;

    private:
        std::vector<Point> points;
        Color fill;
    };

    class Rect : public SVGElement
    {
    public:
        Rect(const Point& x, const Point& y, const double& rx, const double& ry, const double& width, const double& height, std::string stroke);
        void draw(PNGImage &img) const override;
    private:
        Point x,y;
        double rx,ry, width, height;
        std::string stroke;
    };


    // Translate a point
    Point Point::translate(const Point &t) const {
        int dx, dy;
        std::cin >> dx >> dy;
        Point translatedPoint;
        translatedPoint.x = (t).x + dx;
        translatedPoint.y = (t).y + dy;
        return translatedPoint;
    }

    // Rotate a point
    Point Point::rotate(const Point &origin, int degrees) const {
        double radians = degrees * 3.1416 / 180.0;

        int translatedX = this->x - origin.x;
        int translatedY = this->y - origin.y;

        int rotatedX = translatedX * cos(radians) - translatedY * sin(radians);
        int rotatedY = translatedX * sin(radians) + translatedY * cos(radians);

        rotatedX += origin.x;
        rotatedY += origin.y;

        return Point{rotatedX, rotatedY};
    }

    // Scale a point
    Point Point::scale(const Point &origin, int v) const {
        int translatedX = this->x - origin.x;
        int translatedY = this->y - origin.y;

        int scaledX = translatedX * v;
        int scaledY = translatedY * v;
        scaledX += origin.x;
        scaledY += origin.y;

        return Point{scaledX, scaledY};
    }
}
#endif __svg_SVGElements_hpp__


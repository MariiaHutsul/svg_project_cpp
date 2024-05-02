//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

namespace svg
{
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


     class Circle: public Ellipse 
    {
    public:
        Circle(const Point &center, const double &r, const Color &fill);
        void draw(PNGImage &img) const override;
    };


    class Polyline : public SVGElement
    {
    public:
        Polyline(const std::vector<Point> &points, const Color& stroke);
        void draw(PNGImage &img) const override;

    private:
        std::vector<Point> points;
        Color stroke;
    };

    class Line : public SVGElement
    {
    public:
        Line(const Point& p1, const Point& p2, const Color &stroke);
        void draw(PNGImage &img) const override;
    private:
        Point p1, p2;
        Color stroke;
    };


    class Polygon : public SVGElement
    {
    public:
        Polygon(const std::vector<Point> &points, const Color &fill);
        void draw(PNGImage &img) const override;

    private:
        std::vector<Point> points;
        Color fill;
    };

    class Rect : public SVGElement
    {
    public:
        Rect(const Point &topLeft, const double &width, const double &height, const Color &stroke);
        void draw(PNGImage &img) const override;

    private:
        Point topLeft;
        double width, height;
        Color stroke;
    };
}
#endif __svg_SVGElements_hpp__


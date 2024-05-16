//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

#include <iostream>  // New
#include <string>
using namespace std; // New

namespace svg 
{
    // Base class for all SVG elements
    class SVGElement
    {

    public:
        // Methods to be implemented by derived classes
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual Color get_color() = 0;
        virtual std::vector<Point> get_points() = 0;
        virtual Point get_center() = 0;
        virtual Point get_radius() = 0;
        virtual const std::string get_name() = 0;
        virtual SVGElement* clone() = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);


    // Ellipse class derived from SVGElement
    class Ellipse : public SVGElement
    {
    public:
        // Constructor initializing fill color, center point and radius
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        // Destructor
        ~Ellipse() override final;

        // Overridden methods from SVGElement
        void draw(PNGImage &img) const override;
        Color get_color() override final{return fill;};
        Point get_center() override final{return center;};
        Point get_radius() override final{return radius;};
        std::vector<Point> get_points() override final{return {};};
        const std::string get_name() override final{return "ellipse";};
        Ellipse* clone() override final {return new Ellipse(fill, center, radius);}

    private:
        Color fill;
        Point center;
        Point radius;
    };

    // Polyline class derived from SVGElement
    class Polyline : public SVGElement
    {
    public:
        // Constructor initializing points and stroke color
        Polyline(const std::vector<Point> &points, const Color& stroke);
        // Destructor
        ~Polyline() override final;

        // Overridden methods from SVGElement
        void draw(PNGImage &img) const override;
        Color get_color() override final{return stroke;};
        std::vector<Point> get_points() override final{return points;};
        Point get_center() override final{return {0,0};};
        Point get_radius() override final{return {0,0};};
        const std::string get_name() override final{return "polyline";};
        Polyline* clone() override final {return new Polyline(points, stroke);}
        
    private:
        std::vector<Point> points;
        Color stroke;
    };

    // Polygon class derived from SVGElement
    class Polygon : public SVGElement
    {
    public:
        // Constructor initializing points and fill color
        Polygon(const std::vector<Point> &points, const Color &fill);
        // Destructor
        ~Polygon() override final;

        // Overridden methods from SVGElement
        void draw(PNGImage &img) const override;
        Color get_color() override final{return fill;};
        std::vector<Point> get_points() override final{return points;};
        Point get_center() override final{return {0,0};};
        Point get_radius() override final{return {0,0};};
        const std::string get_name() override final{return "polygon";};
        Polygon* clone() override final {return new Polygon(points, fill);}

    private:
        std::vector<Point> points;
        Color fill;
    };
}
#endif

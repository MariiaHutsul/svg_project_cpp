//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

#include <iostream>
#include <string>
using namespace std;

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual Color get_color() = 0;
        virtual std::vector<Point> get_points() = 0;
        virtual Point get_center() = 0;
        virtual Point get_radius() = 0;
        virtual const std::string get_name() = 0;
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
        ~Ellipse() override final;
        void draw(PNGImage &img) const override;
        Color get_color() override final{return fill;};
        Point get_center() override final{return center;};
        Point get_radius() override final{return radius;};
        std::vector<Point> get_points() override final{return {};};
        const std::string get_name() override final{return "ellipse";};

    private:
        Color fill;
        Point center;
        Point radius;
    };

    class Polyline : public SVGElement
    {
    public:
        Polyline(const std::vector<Point> &points, const Color& stroke);
        ~Polyline() override final;
        void draw(PNGImage &img) const override;
        Color get_color() override final{return stroke;};
        std::vector<Point> get_points() override final{return points;};
        Point get_center() override final{return {0,0};};
        Point get_radius() override final{return {0,0};};
        const std::string get_name() override final{return "polyline";};
        
    private:
        std::vector<Point> points;
        Color stroke;
    };

    class Polygon : public SVGElement
    {
    public:
        Polygon(const std::vector<Point> &points, const Color &fill);
        ~Polygon() override final;
        void draw(PNGImage &img) const override;
        Color get_color() override final{return fill;};
        std::vector<Point> get_points() override final{return points;};
        Point get_center() override final{return {0,0};};
        Point get_radius() override final{return {0,0};};
        const std::string get_name() override final{return "polygon";};

    private:
        std::vector<Point> points;
        Color fill;
    };
}
#endif

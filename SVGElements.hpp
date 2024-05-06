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
        virtual Color get_color() = 0;
        virtual std::vector<Point> get_points() = 0;
        virtual Point get_center() = 0;
        virtual Point get_radius() = 0;
        virtual const string get_name() = 0;
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
        Color get_color() override final{return fill;};
        Point get_center() override final{return center;};
        Point get_radius() override final{return radius;};
        std::vector<Point> get_points() override final;
        const string get_name() override final{return "ellipse";};

    private:
        Color fill;
        Point center;
        Point radius;
    };

    class Polyline : public SVGElement
    {
    public:
        Polyline(const std::vector<Point> &points, const Color& stroke);
        void draw(PNGImage &img) const override;
        Color get_color() override final{return stroke;};
        std::vector<Point> get_points() override final{return points;};
        Point get_center() override final;
        Point get_radius() override final;
        const string get_name() override final{return "polyline";};
        
    private:
        std::vector<Point> points;
        Color stroke;
    };

    class Polygon : public SVGElement
    {
    public:
        Polygon(const std::vector<Point> &points, const Color &fill);
        void draw(PNGImage &img) const override;
        Color get_color() override final{return fill;};
        std::vector<Point> get_points() override final{return points;};
        Point get_center() override final;
        Point get_radius() override final;
        const string get_name() override final{return "polygon";};

    private:
        std::vector<Point> points;
        Color fill;
    };

     Point Polyline::get_center() {
        // Calculate the centroid of the polyline
        int sum_x = 0, sum_y = 0;
        for (const auto &point : points) {
            sum_x += point.x;
            sum_y += point.y;
        }
        return {sum_x / points.size(), sum_y / points.size()};
    }

    Point Polyline::get_radius() {
        return {0, 0};
    }

    void Polyline::draw(PNGImage &img) const {
         void draw_line(const Point &a, const Point &b, const Color &c);

    }

    std::vector<Point> Polygon::get_points() {
        return points;
    }

    Point Polygon::get_center() {
        int sum_x = 0, sum_y = 0;
        for (const auto &point : points) {
            sum_x += point.x;
            sum_y += point.y;
        }
        return {sum_x / points.size(), sum_y / points.size()};
    }

    Point Polygon::get_radius() {
        return {0, 0};
    }

    void Polygon::draw(PNGImage &img) const {
        void draw_polygon(const std::vector<Point> &points, const Color &fill);

    }

    std::vector<Point> Ellipse::get_points() {
        return {};
    }

    Point Ellipse::get_center() {
        return center;
    }

    Point Ellipse::get_radius() {
        return radius;
    }

    void Ellipse::draw(PNGImage &img) const {
        void draw_ellipse(const Point &center, const Point &radius, const Color &fill);

    }
}

#endif __svg_SVGElements_hpp__


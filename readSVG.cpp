
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <sstream>
#include <algorithm>

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        // TODO complete code -->

        for (XMLElement* child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
            // Getting the shape for the current node
            string shape = child->Name();

            // svg_element inicialization
            SVGElement* svg_element = new SVGElement();

            // Create the correct class for the shape
            if(shape == "ellipse"){
                // Getting the attributes for ellipse
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int rx = child->IntAttribute("rx");
                int ry = child->IntAttribute("ry");
                const char* color = child->Attribute("fill");

                Color fill = parse_color(color);
                Point center = {cx, cy};
                Point radius = {rx,ry};
                // Creating ellipse
                SVGElement* svg_element = new Ellipse(fill,center, radius);
            }
            else if(shape == "circle"){
                // Getting the attributes for circle
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int r = child->IntAttribute("r");
                const char* color = child->Attribute("fill");

                Color fill = parse_color(color);
                Point center = {cx, cy};
                Point radius = {r,r};
                // Creating circle
                SVGElement* svg_element = new Ellipse(fill,center, radius);  // Circle is a particular type of Ellipse
            }
            else if(shape == "polyline"){
                // Getting the attributes for polyline
                string p = child ->Attribute("points");
                istringstream in(p);
                std::vector<Point> points;
                float x, y;
                char c;
                while (in >> x >> c >> y) {     // Using istringstream to get the coordinates of the point on the string
                    Point p = {x,y};            
                    points.push_back(p);
                }
    
                string stroke_ = child->Attribute("stroke");
                Color stroke = parse_color(stroke_);
                // Creating polyline
                SVGElement* svg_element = new Polyline(points, stroke);
            }
            else if(shape == "line"){
                // Getting the attributes for line
                int x1 = child->IntAttribute("x1");
                int x2 = child->IntAttribute("x2");
                int y1 = child->IntAttribute("y1");
                int y2 = child->IntAttribute("y2");
                Point p1 = {x1,y1};
                Point p2 = {x2,y2};
                std::vector<Point> points = {p1,p2};
    
                string stroke_ = child->Attribute("stroke");
                Color stroke = parse_color(stroke_);
                // Creating line
                SVGElement* svg_element = new Polyline(points, stroke);     // Line is a particular type of Polyline
            }
            else if(shape == "polygon"){
                // Getting the attributes for polygon
                string p = child ->Attribute("points");
                istringstream in(p);
                std::vector<Point> points;
                int x, y;
                char c;
                while (in >> x >> c >> y) {     // Using istringstream to get the coordinates of the point on the string
                    Point p = {x,y};
                    points.push_back(p);
                }
    
                string fill_ = child->Attribute("fill");
                Color fill = parse_color(fill_);
                // Creating polygon
                SVGElement* svg_element = new Polygon(points, fill);
            }
            else if(shape == "rectangle"){
                // Getting the attributes for rectangle
                int x = stoi(child->Attribute("x"));
                int y = stoi(child->Attribute("y"));
                int width = stoi(child->Attribute("width"));
                int height = stoi(child->Attribute("height"));
                Point p1 = {x,y};
                Point p2 = {x + width, y};
                Point p3 = {x + width, y + height};
                Point p4 = {x, y + height};
                std::vector<Point> points = {p1,p2,p3,p4};
    
                string fill_ = child->Attribute("fill");
                Color fill = parse_color(fill_);
                // Creating rectangle
                SVGElement* svg_element = new Polygon(points, fill);    // Rectangle is a particular type of Polygon
            }

            // Check for transformations

            if (child->Attribute("transform")){
                string transformation_ = child->Attribute("transform");
                size_t i = transformation_.find("(");
                string transformation = transformation_.substr(0, i);
                transformation_.erase(0,i);     // Only keep the value of transformation on the string
                replace(transformation_.begin(), transformation_.end(), ',', ' ');

                if(transformation == "translate"){
                    // For Polylines and Polygons translate all points in vector
                    // For Ellipses it's only required to translate the center
                    // Getting values
                    istringstream in(transformation_);
                    int x, y;
                    in >> x >> y;
                    Point t = {x,y};

                    if(shape == "ellipse" or shape == "circle"){
                        Color fill = svg_element.get_color();
                        Point center = svg_element.get_center();
                        Point radius = svg_element.get_radius();
                        delete svg_element;
                        center.translate(t);
                        SVGElement* svg_element = new Ellipse(fill,center, radius); // Creating new svg_element with the updated center
                    }
                    else{
                        Color c = svg_element.get_color();
                        std::vector<Point> points = svg_element.get_points();
                        delete svg_element;
                        for( Point& p : points){
                            p.translate(t);
                        }
                        if(shape == "rectangle" or shape == "polygon"){SVGElement* svg_element = new Polygon(points, c);}
                        else{SVGElement* svg_element = new Polyline(points, c);}        // Creating new svg_element with the updated points
                    }
                }
                else if(transformation == "rotate"){
                    // Check for origin and for all shapes rotate points with defined function

                }
                else if(transformation == "scale"){
                    // Check for origin and scale all points for polygons and polylines, be carefull with radius for ellipses
                    
                }
            }

            // Add object to vector
            svg_elements.push_back(svg_element);
            
        }
        
    }
}
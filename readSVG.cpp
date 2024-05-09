
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <sstream>
#include <algorithm>

using namespace std;
using namespace tinyxml2;

namespace svg
{   
    void shape_translate(string shape, SVGElement*& svg_element, string value){
        // For Polylines and Polygons translate all points in vector
        // For Ellipses it's only required to translate the center

        // Getting translation value
        std::istringstream in(value);
        int x, y;
        in >> x >> y;
        Point t = {x,y};

        // Apply the transformation
        if(shape == "ellipse" or shape == "circle"){
            Color fill = svg_element -> get_color();
            Point center = svg_element -> get_center();
            Point radius = svg_element -> get_radius();
            center.translate(t);
            svg_element = new Ellipse(fill,center, radius); // Creating new svg_element with the updated center
        }
        else{
            Color c = svg_element -> get_color();
            std::vector<Point> points = svg_element -> get_points();
            delete svg_element;
            for( Point& p : points){
                p.translate(t);
            }
            if(shape == "rectangle" or shape == "polygon"){svg_element = new Polygon(points, c);}
            else{svg_element = new Polyline(points, c);}        // Creating new svg_element with the updated points
        }
    }

    void shape_rotate(string shape, SVGElement*& svg_element, string value, Point origin){
        //  For all shapes rotate points with defined function

        // Getting the angle to rotate
        std::istringstream in(value);
        int angle;
        in >> angle;

        // Apply the transformation
        if(shape == "ellipse" or shape == "circle"){
            Color fill = svg_element -> get_color();
            Point center = svg_element -> get_center();
            Point radius = svg_element -> get_radius();
            center.rotate(origin, angle);
            svg_element = new Ellipse(fill, center, radius);    // Creating new svg_element with the updated center
        }
        else{
            Color c = svg_element -> get_color();
            std::vector<Point> points = svg_element -> get_points();
            for( Point& p : points){
                p.rotate(origin, angle);
            }
            if(shape == "rectangle" or shape == "polygon"){svg_element = new Polygon(points, c);}
            else{svg_element = new Polyline(points, c);}        // Creating new svg_element with the updated points
        } 
    }
    
    void shape_scale(string shape, SVGElement*& svg_element, string value, Point origin){
        // Scale all points for polygons and polylines, for ellipses scale the center as well as the radius

        // Getting the scaling factor
        std::istringstream in(value);
        int factor;
        in >> factor;

        // Apply the transformation
        if(shape == "ellipse" or shape == "circle"){
            Color fill = svg_element -> get_color();
            Point center = svg_element -> get_center();
            Point radius = svg_element -> get_radius();
            center.scale(origin, factor);
            radius.scale(origin,factor);
            svg_element = new Ellipse(fill, center, radius);    // Creating new svg_element with the updated center and radius
        }
        else{
            Color c = svg_element -> get_color();
            std::vector<Point> points = svg_element -> get_points();
            for( Point& p : points){
                p.scale(origin, factor);
            }
            if(shape == "rectangle" or shape == "polygon"){svg_element = new Polygon(points, c);}
            else{svg_element = new Polyline(points, c);}        // Creating new svg_element with the updated points
        } 
    }

    void groups(XMLElement *group,vector<SVGElement*>& parent_group,string group_transformation = "", Point group_origin = {0,0}){
        // Process the current group
        // Vector to store elements of the group

        vector<SVGElement*> group_elements;

        // Process children of the current group
        for (XMLElement* child = group->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
            
            // Getting the shape for the current node
            string shape = child->Name();

            if (shape == "g") {
                // Check for transformation
                string transformation_;
                if (child->Attribute("transform")){
                    transformation_ = child->Attribute("transform");
                }

                // Check for origin
                Point o;
                if(child->Attribute("transform-origin")){
                    string origin = child->Attribute("transform-origin");
                    std::istringstream in(origin);
                    int x, y;
                    in >> x >> y;
                    o = {x,y};
                }
                else{
                    o = {0,0};    // Default transform origin
                }
                
                // Recursively process nested groups
                groups(child,group_elements,transformation_, o);
            } 
            else {
                // Process other shapes within the group

                // svg_element inicialization
                SVGElement* svg_element = nullptr;

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
                    svg_element = new Ellipse(fill,center, radius);
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
                    svg_element = new Ellipse(fill,center, radius);  // Circle is a particular type of Ellipse
                }
                else if(shape == "polyline"){
                    // Getting the attributes for polyline
                    string p = child ->Attribute("points");
                    std::istringstream in(p);
                    std::vector<Point> points;
                    int x, y;
                    char c;
                    while (in >> x >> c >> y) {     // Using istringstream to get the coordinates of the point on the string
                        Point p = {x,y};            
                        points.push_back(p);
                    }
        
                    string stroke_ = child->Attribute("stroke");
                    Color stroke = parse_color(stroke_);
                    // Creating polyline
                    svg_element = new Polyline(points, stroke);
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
                    svg_element = new Polyline(points, stroke);     // Line is a particular type of Polyline
                }
                else if(shape == "polygon"){
                    // Getting the attributes for polygon
                    string p = child ->Attribute("points");
                    std::istringstream in(p);
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
                    svg_element = new Polygon(points, fill);
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
                    svg_element = new Polygon(points, fill);    // Rectangle is a particular type of Polygon
                }

                // Check for transformations

                if (child->Attribute("transform")){
                    string transformation_ = child->Attribute("transform");
                    size_t i = transformation_.find("(");
                    string transformation = transformation_.substr(0, i);
                    transformation_.erase(0,i);     // Only keep the value of transformation on the string
                    std::replace(transformation_.begin(), transformation_.end(), ',', ' ');

                    // Check for origin
                    Point o;
                    if(child->Attribute("transform-origin")){
                        string origin = child->Attribute("transform-origin");
                        std::istringstream in(origin);
                        int x, y;
                        in >> x >> y;
                        o = {x,y};
                    }
                    else{
                        o = {0,0};    // Default transform origin
                    }

                    if(transformation == "translate"){
                        shape_translate(shape, svg_element, transformation_);
                    }
                    else if(transformation == "rotate"){
                        shape_rotate(shape, svg_element, transformation_, o);
                    }
                    else if(transformation == "scale"){
                        shape_scale(shape, svg_element, transformation_, o);
                    }
                    
                }
                // Add object to vector
                if(svg_element != nullptr){group_elements.push_back(svg_element);}
                
            }
        }
        for(auto& element : group_elements) {
            if(group_transformation == "translate"){
                shape_translate(element -> get_name(), element, group_transformation);
            }
            else if(group_transformation == "rotate"){
                shape_rotate(element -> get_name(), element, group_transformation, group_origin);
            }
            else if(group_transformation == "scale"){
                shape_scale(element -> get_name(), element, group_transformation, group_origin);
            }
            if(element != nullptr){parent_group.push_back(element);}
        }
        
    }

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
            SVGElement* svg_element = nullptr;

            // Check for groups
            if(shape == "g"){
                // Check for transformation
                string transformation_;
                if (child->Attribute("transform")){
                    transformation_ = child->Attribute("transform");
                }

                // Check for origin
                Point o;
                if(child->Attribute("transform-origin")){
                    string origin = child->Attribute("transform-origin");
                    std::istringstream in(origin);
                    int x, y;
                    in >> x >> y;
                    o = {x,y};
                }
                else{
                    o = {0,0};    // Default transform origin
                }
                
                // Function to recursively process nested groups
                groups(child,svg_elements,transformation_, o);
            }

            // Create the correct class for the shape
            else if(shape == "ellipse"){
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
                svg_element = new Ellipse(fill,center, radius);
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
                svg_element = new Ellipse(fill,center, radius);  // Circle is a particular type of Ellipse
            }
            else if(shape == "polyline"){
                // Getting the attributes for polyline
                string p = child ->Attribute("points");
                istringstream in(p);
                std::vector<Point> points;
                int x, y;
                char c;
                while (in >> x >> c >> y) {     // Using istringstream to get the coordinates of the point on the string
                    Point p = {x,y};            
                    points.push_back(p);
                }
    
                string stroke_ = child->Attribute("stroke");
                Color stroke = parse_color(stroke_);
                // Creating polyline
                svg_element = new Polyline(points, stroke);
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
                svg_element = new Polyline(points, stroke);     // Line is a particular type of Polyline
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
                svg_element = new Polygon(points, fill);
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
                svg_element = new Polygon(points, fill);    // Rectangle is a particular type of Polygon
            }

            // Check for transformations

            if (child->Attribute("transform")){
                string transformation_ = child->Attribute("transform");
                size_t i = transformation_.find("(");
                string transformation = transformation_.substr(0, i);
                transformation_.erase(0,i);     // Only keep the value of transformation on the string
                std::replace(transformation_.begin(), transformation_.end(), ',', ' ');

                // Check for origin
                Point o;
                if(child->Attribute("transform-origin")){
                    string origin = child->Attribute("transform-origin");
                    istringstream in(origin);
                    int x, y;
                    in >> x >> y;
                    o = {x,y};
                }
                else{
                    o = {0,0};    // Default transform origin
                }

                if(transformation == "translate"){
                    shape_translate(shape, svg_element, transformation_);
                }
                else if(transformation == "rotate"){
                    shape_rotate(shape, svg_element, transformation_, o);
                }
                else if(transformation == "scale"){
                    shape_scale(shape, svg_element, transformation_, o);
                }
            }

            // Add object to vector
            if(svg_element != nullptr){svg_elements.push_back(svg_element);}
        }
    }
}
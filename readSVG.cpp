
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <string>
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
            Point c = center.translate(t);
            delete svg_element;
            svg_element = new Ellipse(fill,c, radius); // Creating new svg_element with the updated center
        }
        else{
            Color c = svg_element -> get_color();
            std::vector<Point> points0 = svg_element -> get_points();
            std::vector<Point> points;
            for( Point& p0 : points0){
                Point p = p0.translate(t);
                points.push_back(p);
            }
            delete svg_element;
            if(shape == "rect" or shape == "polygon"){svg_element = new Polygon(points, c);}
            else{svg_element = new Polyline(points, c);}        // Creating new svg_element with the updated points
        }
    }

    void shape_rotate(string shape, SVGElement*& svg_element, string value, Point origin){
        //  For all shapes rotate points with defined function

        // Getting the angle to rotate
        int angle;
        std::istringstream(value) >> angle;

        // Apply the transformation
        if(shape == "ellipse" or shape == "circle"){
            Color fill = svg_element -> get_color();
            Point center = svg_element -> get_center();
            Point radius = svg_element -> get_radius();
            Point c = center.rotate(origin, angle);
            delete svg_element;
            svg_element = new Ellipse(fill, c, radius);    // Creating new svg_element with the updated center
        }
        else{
            Color c = svg_element -> get_color();
            std::vector<Point> points0 = svg_element -> get_points();
            std::vector<Point> points;
            for( Point& p0 : points0){
                Point p = p0.rotate(origin, angle);
                points.push_back(p);
            }
            delete svg_element;
            if(shape == "rect" or shape == "polygon"){svg_element = new Polygon(points, c);}
            else{svg_element = new Polyline(points, c);}        // Creating new svg_element with the updated points
        } 
    }
    
    void shape_scale(string shape, SVGElement*& svg_element, string value, Point origin){
        // Scale all points for polygons and polylines, for ellipses scale the center as well as the radius

        // Getting the scaling factor
        // Convert the substring to an integer
        int factor;
        std::istringstream(value) >> factor;

        // Apply the transformation
        if(shape == "ellipse" or shape == "circle"){
            Color fill = svg_element -> get_color();
            Point center = svg_element -> get_center();
            Point radius = svg_element -> get_radius();
            Point c = center.scale(origin, factor);
            Point r = {radius.x * factor, radius.y * factor};
            delete svg_element;
            svg_element = new Ellipse(fill, c, r);    // Creating new svg_element with the updated center and radius
        }
        else{
            Color c = svg_element -> get_color();
            std::vector<Point> points0 = svg_element -> get_points();
            std::vector<Point> points;
            for( Point& p0 : points0){
                Point p = p0.scale(origin, factor);
                points.push_back(p);
            }
            delete svg_element;
            if(shape == "rect" or shape == "polygon"){svg_element = new Polygon(points, c);}
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

            // svg_element inicialization
            SVGElement* svg_element = nullptr;
            
            // Check for groups
            if(shape == "g"){
                // Check for transformation
                string transformation_;
                if (child->Attribute("transform")){
                    transformation_ = child->Attribute("transform");
                }
                else{
                    transformation_ = "";
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

                delete svg_element;
                // Function to recursively process nested groups
                groups(child,group_elements,transformation_, o);
            }
            else {
                // Process other shapes within the group
                // Create the correct class for the shape

                if(shape == "ellipse"){
                    // Getting the attributes for ellipse
                    int cx = child->IntAttribute("cx");
                    int cy = child->IntAttribute("cy");
                    int rx = child->IntAttribute("rx");
                    int ry = child->IntAttribute("ry");
                    string color = child->Attribute("fill");

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
                    string color = child->Attribute("fill");
            
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
                    string s = child -> Attribute("points");
                    std::vector<Point> points;
                    istringstream in(s);
                    
                    char c;
                    Point p;

                    while (in >> p.x >> c >> p.y) {  // Using istringstream to get the coordinates of the points on the string
                        points.push_back(p);
                    }
        
                    string fill_ = child->Attribute("fill");
                    Color fill = parse_color(fill_);
                    // Creating polygon
                    svg_element = new Polygon(points, fill);
                }
                else if(shape == "rect"){
                    // Getting the attributes for rectangle
                    int x = child->IntAttribute("x");
                    int y = child->IntAttribute("y");
                    int width = child->IntAttribute("width");
                    int height = child->IntAttribute("height");
                    Point p1 = {x,y};
                    Point p2 = {x+ width - 1, y };
                    Point p3 = {x + width - 1, y + height - 1};
                    Point p4 = {x , y + height - 1};
                    std::vector<Point> points = {p1,p2,p3,p4};
        
                    string fill_ = child->Attribute("fill");
                    Color fill = parse_color(fill_);
                    // Creating rectangle
                    svg_element = new Polygon(points, fill);    // Rectangle is a particular type of Polygon
                }

                // Check for transformations

                if (child->Attribute("transform")){
                    string transformation_ = child->Attribute("transform");
                    std::replace(transformation_.begin(), transformation_.end(), ',', ' ');
                    size_t startPos = transformation_.find('(');
                    size_t endPos = transformation_.find(')');

                    // Extract the substring between '(' and ')'
                    std::string value_ = transformation_.substr(startPos + 1, endPos - startPos - 1);

                    // Extract the substring before '('
                    std::string transformation = transformation_.substr(0, startPos);

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
                        shape_translate(shape, svg_element, value_);
                    }
                    else if(transformation == "rotate"){
                        shape_rotate(shape, svg_element, value_, o);
                    }
                    else if(transformation == "scale"){
                        shape_scale(shape, svg_element, value_, o);
                    }
                }
                group_elements.push_back(svg_element);
            }
        }

        string gp = "";
        string value_;
        if (group_transformation != ""){
            std::replace(group_transformation.begin(), group_transformation.end(), ',', ' ');
            size_t startPos = group_transformation.find('(');
            size_t endPos = group_transformation.find(')');

            // Extract the substring between '(' and ')'
            value_ = group_transformation.substr(startPos + 1, endPos - startPos - 1);

            // Extract the substring before '('
            gp = group_transformation.substr(0, startPos);
        }
        for(auto& element : group_elements) {
            if(gp == "translate"){
                shape_translate(element -> get_name(), element, value_);
            }
            else if(gp == "rotate"){
                shape_rotate(element -> get_name(), element, value_, group_origin);
            }
            else if(gp == "scale"){
                shape_scale(element -> get_name(), element, value_, group_origin);
            }
            parent_group.push_back(element);
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
                else{
                    transformation_ = "";
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

                delete svg_element;
                // Function to recursively process nested groups
                groups(child,svg_elements,transformation_, o);
            }

            // Create the correct class for the shape
            else{
                if(shape == "ellipse"){
                    // Getting the attributes for ellipse
                    int cx = child->IntAttribute("cx");
                    int cy = child->IntAttribute("cy");
                    int rx = child->IntAttribute("rx");
                    int ry = child->IntAttribute("ry");
                    string color = child->Attribute("fill");

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
                    string color = child->Attribute("fill");
            
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
                    string s = child -> Attribute("points");
                    std::vector<Point> points;
                    istringstream in(s);
                    
                    char c;
                    Point p;

                    while (in >> p.x >> c >> p.y) {  // Using istringstream to get the coordinates of the points on the string
                        points.push_back(p);
                    }
        
                    string fill_ = child->Attribute("fill");
                    Color fill = parse_color(fill_);
                    // Creating polygon
                    svg_element = new Polygon(points, fill);
                }
                else if(shape == "rect"){
                    // Getting the attributes for rectangle
                    int x = child->IntAttribute("x");
                    int y = child->IntAttribute("y");
                    int width = child->IntAttribute("width");
                    int height = child->IntAttribute("height");
                    Point p1 = {x,y};
                    Point p2 = {x+ width - 1, y };
                    Point p3 = {x + width - 1, y + height - 1};
                    Point p4 = {x , y + height - 1};
                    std::vector<Point> points = {p1,p2,p3,p4};
        
                    string fill_ = child->Attribute("fill");
                    Color fill = parse_color(fill_);
                    // Creating rectangle
                    svg_element = new Polygon(points, fill);    // Rectangle is a particular type of Polygon
                }

                // Check for transformations

                if (child->Attribute("transform")){
                    string transformation_ = child->Attribute("transform");
                    std::replace(transformation_.begin(), transformation_.end(), ',', ' ');
                    size_t startPos = transformation_.find('(');
                    size_t endPos = transformation_.find(')');

                    // Extract the substring between '(' and ')'
                    std::string value_ = transformation_.substr(startPos + 1, endPos - startPos - 1);

                    // Extract the substring before '('
                    std::string transformation = transformation_.substr(0, startPos);

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
                        shape_translate(shape, svg_element, value_);
                    }
                    else if(transformation == "rotate"){
                        shape_rotate(shape, svg_element, value_, o);
                    }
                    else if(transformation == "scale"){
                        shape_scale(shape, svg_element, value_, o);
                    }
                }
                svg_elements.push_back(svg_element);
            }
        }
    }
}
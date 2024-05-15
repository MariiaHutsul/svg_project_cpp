
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

using namespace std;
using namespace tinyxml2;

namespace svg
{   
    // Map to store the ids and corresponding references
    std::map<std::string, std::vector<SVGElement*>> id_store;
    
    // Function to get the point in a string from the attribute "transform-origin"
    Point t_origin(string origin){
        istringstream in(origin);
        int x, y;
        in >> x >> y;
        Point o = {x,y};
        return o;
    }

    // Function to get the transformation and corresponding value from the attribute "transform"
    pair<string, string> t_and_value(string t){
        std::replace(t.begin(), t.end(), ',', ' ');
        size_t startPos = t.find('(');
        size_t endPos = t.find(')');

        // Extract the substring between '(' and ')'
        std::string value_ = t.substr(startPos + 1, endPos - startPos - 1);

        // Extract the substring before '('
        std::string transformation = t.substr(0, startPos);
        pair<string, string> p = make_pair(transformation, value_);
        return p;
    }

    // Function called to translate a shape 
    void shape_translate(string shape, SVGElement*& svg_element, string value){
        // For Polylines and Polygons translate all points in vector
        // For Ellipses it's only required to translate the center, as it is defined by center and radius (and color)

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

    // Function called to rotate a shape 
    void shape_rotate(string shape, SVGElement*& svg_element, string value, Point origin){

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
    
    // Function called to scale a shape 
    void shape_scale(string shape, SVGElement*& svg_element, string value, Point origin){
        // Scale all points for polygons and polylines, for ellipses scale the center as well as the radius

        // Getting the scaling factor
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

    void groups(XMLElement *group,vector<SVGElement*>& parent_group,string group_transformation = "", Point group_origin = {0,0}, string group_id = ""){
        // Process the current group

        // Vector to store elements of the group
        vector<SVGElement*> group_elements;

        // Process children of the current group
        for (XMLElement* child = group->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
            // Getting the shape for the current node
            string shape = child->Name();
            
            // Check for groups
            if(shape == "g"){
                // Check for group transformation
                string transformation_;
                if (child->Attribute("transform")){
                    transformation_ = child->Attribute("transform");
                }
                else{
                    transformation_ = "";
                }

                // Check for group origin
                Point o = {0,0};    // Default transform origin
                if(child->Attribute("transform-origin")){
                    o = t_origin(child->Attribute("transform-origin"));
                }

                // Store the id in a variable
                string id;
                if(child -> Attribute("id")){
                    id = child -> Attribute("id");
                }
                else{
                    id = "";    // No id
                }

                // Function to recursively process nested groups
                groups(child,group_elements,transformation_, o,id);
            }
            // Process other shapes within the group
            else if (shape != "use"){

                // svg_element inicialization
                SVGElement* svg_element = nullptr;

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

                    // Get the transformation and its value
                    string transformation_ = child->Attribute("transform");
                    
                    pair<string, string> p = t_and_value(transformation_);

                    string transformation = p.first;
                    string value_ = p.second;

                    // Check for origin
                    Point o = {0,0};    // Default transform origin
                    if(child->Attribute("transform-origin")){
                        o = t_origin(child->Attribute("transform-origin"));
                    }

                    // Apply the corresponding operation
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

                // Reading id, if it exists 
                if(child -> Attribute("id")){
                    string id = child -> Attribute("id");
                    id = "#" + id;     // Adding '#' in order to compare the id to href, that has a '#' in the beginning
                    vector<SVGElement*> element;
                    auto e = svg_element -> clone();
                    element.push_back(e);
                    id_store.insert({group_id , element});  
                }
            }

            // Reading a "use" - The only remaining case for this project
            else{
                string ref = child -> Attribute("href");    // Getting the corresponding reference

                // Storing all the pointers in a new vector as copies in order to avoid modifying them
                vector<SVGElement*> c;

                for (SVGElement* ptr : id_store[ref]) {
                    string s = ptr -> get_name();
                    SVGElement* newPtr;
                    if(s == "ellipse"){ newPtr = new Ellipse(ptr -> get_color(), ptr -> get_center(), ptr -> get_radius());}
                    else if(s == "polyline"){ newPtr = new Polyline(ptr -> get_points(), ptr -> get_color());}
                    else { newPtr = new Polygon(ptr -> get_points(), ptr -> get_color());}

                    c.push_back(newPtr);
                }

                if (child->Attribute("transform")){
                    // Get the transformation and its value
                    string transformation_ = child->Attribute("transform");
                    
                    pair<string, string> p = t_and_value(transformation_);

                    string transformation = p.first;
                    string value_ = p.second;

                    // Check for origin
                    Point o = {0,0};    // Default transform origin
                    if(child->Attribute("transform-origin")){
                        o = t_origin(child->Attribute("transform-origin"));
                    }

                    for(auto& e : c){
                        if(transformation == "translate"){
                            shape_translate(e -> get_name(), e, value_);
                        }
                        else if(transformation == "rotate"){
                            shape_rotate(e -> get_name(), e, value_, o);
                        }
                        else if(transformation == "scale"){
                            shape_scale(e -> get_name(), e, value_, o);
                        }
                    }
                }

                if(child -> Attribute("id")){
                    string id = child -> Attribute("id");
                    id = "#" + id;      // Adding '#' in order to compare the id to href, that always has a '#' in the beginning
                    vector<SVGElement*> copy_elements;
                    for(auto e_ : c){
                        auto e = e_ -> clone();
                        copy_elements.push_back(e);
                    }
                    id_store.insert({id , copy_elements});
                }

                for(auto e : c){
                    group_elements.push_back(e);
                }
            }
        }

        // Getting and applying the group transformation
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

        // Storing group id, if it exists
        if(group_id != ""){
            group_id = "#" + group_id;      // Adding '#' in order to compare the id to href, that always has a '#' in the beginning
            vector<SVGElement*> copy_elements;
            for(auto e_ : group_elements){
                auto e = e_ -> clone();
                copy_elements.push_back(e);
            }
            id_store.insert({group_id , copy_elements});
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
                Point o = {0,0};    // Default transform origin
                if(child->Attribute("transform-origin")){
                    o = t_origin(child->Attribute("transform-origin"));
                }

                // Store the id in a variable
                string id;
                if(child -> Attribute("id")){
                    id = child -> Attribute("id");
                }
                else{
                    id = "";    // No id
                }

                // Function to recursively process nested groups
                groups(child,svg_elements,transformation_, o, id);
            }
            else if (shape != "use") {

                // svg_element inicialization
                SVGElement* svg_element = nullptr;

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
                    Point p;

                    while (in >> p.x) {             // Using istringstream to get the coordinates of the points on the string
                        if (in.peek() == ',') {
                             in.ignore();
                        }
                        if (in >> p.y) {
                             points.push_back(p);
                        }
                        if (in.peek() == ','){
                            in.ignore();
                        }
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
                    // Get the transformation and its value
                    string transformation_ = child->Attribute("transform");
                    
                    pair<string, string> p = t_and_value(transformation_);

                    string transformation = p.first;
                    string value_ = p.second;

                    // Check for origin
                    Point o = {0,0};    // Default transform origin
                    if(child->Attribute("transform-origin")){
                        o = t_origin(child->Attribute("transform-origin"));
                    }

                    // Apply the corresponding operation
                    if(transformation == "translate"){
                        shape_translate(svg_element -> get_name(), svg_element, value_);
                    }
                    else if(transformation == "rotate"){
                        shape_rotate(svg_element -> get_name(), svg_element, value_, o);
                    }
                    else if(transformation == "scale"){
                        shape_scale(svg_element -> get_name(), svg_element, value_, o);
                    }
                }
                svg_elements.push_back(svg_element);    // Storing the element

                // Reading id, if it exists
                if(child -> Attribute("id")){
                    string id = child -> Attribute("id");
                    id = "#" + id;      // Adding '#' in order to compare the id to href, that has a '#' in the beginning
                    vector<SVGElement*> element;
                    SVGElement* e = svg_element -> clone();
                    element.push_back(e);
                    id_store.insert({id , element});
                }
            }

            // Reading a "use" - The only remaining case for this project
            else{
                string ref = child -> Attribute("href");    // Getting the corresponding reference
                
                // Storing all the pointers in a new vector as copies in order to avoid modifying them
                vector<SVGElement*> c;

                for (SVGElement* ptr : id_store[ref]) {
                    string s = ptr -> get_name();
                    SVGElement* newPtr;
                    if(s == "ellipse"){ newPtr = new Ellipse(ptr -> get_color(), ptr -> get_center(), ptr -> get_radius());}
                    else if(s == "polyline"){ newPtr = new Polyline(ptr -> get_points(), ptr -> get_color());}
                    else { newPtr = new Polygon(ptr -> get_points(), ptr -> get_color());}

                    c.push_back(newPtr);
                }

                if (child->Attribute("transform")){
                    // Get the transformation and its value
                    string transformation_ = child->Attribute("transform");
                    
                    pair<string, string> p = t_and_value(transformation_);

                    string transformation = p.first;
                    string value_ = p.second;

                    // Check for origin
                    Point o = {0,0};    // Default transform origin
                    if(child->Attribute("transform-origin")){
                        o = t_origin(child->Attribute("transform-origin"));
                    }

                    for(auto& e : c){
                        if(transformation == "translate"){
                            shape_translate(e -> get_name(), e, value_);
                        }
                        else if(transformation == "rotate"){
                            shape_rotate(e -> get_name(), e, value_, o);
                        }
                        else if(transformation == "scale"){
                            shape_scale(e -> get_name(), e, value_, o);
                        }
                    }
                }

                if(child -> Attribute("id")){
                    string id = child -> Attribute("id");
                    id = "#" + id;      // Adding '#' in order to compare the id to href, that always has a '#' in the beginning
                    vector<SVGElement*> copy_elements;
                    for(auto e_ : c){
                        SVGElement* e = e_ -> clone();
                        copy_elements.push_back(e);
                    }
                    id_store.insert({id , copy_elements});
                }

                for(auto e : c){
                    svg_elements.push_back(e);
                }
            }
        }

        // Deleting all the remaining pointers in the ids map to avoid memory leaks
        for (auto& p : id_store){
            auto v = p.second;
            for(auto& e: v){
                delete e;
            }
        }

    }
}
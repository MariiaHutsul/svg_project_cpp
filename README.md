# Programming project

## Group elements


* ### up202306129 - Tiago Catelas Ribeiro Correia
* ### up202310202 - Mariia Hutsul
* ### up202305501 - Martim Oliveira Neves


## Accomplished tasks

### This project concerns the conversion of images from the SVG vectorial format to the PNG raster format.
### The main task in this project is defining a hierarchy of C++ classes corresponding to SVG elements, and code for reading these elements. (  [read more](https://moodle2324.up.pt/pluginfile.php/189678/mod_resource/content/20/index.html) )  
* ### ___The code is compiled with Makefile___
* ### ___In the project directory, you can run [ $  make test ] to compile the project, if everything is Ok, or you will be able to see errors if something goes wrong___
   + ### ___How to understand that it compiled properly?___
   + ### ___In terminal it should be:___
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o test.o test.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o Color.o Color.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o Point.o Point.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o PNGImage.o PNGImage.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o SVGElements.o SVGElements.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o readSVG.o readSVG.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o convert.o convert.cpp
     * ar cr libproj.a external/tinyxml2/tinyxml2.o Color.o Point.o PNGImage.o Point.o SVGElements.o readSVG.o convert.o 
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -o test test.o libproj.a   
* ### ___[ $ ./test  ] Launches the test runner and you will be able to see in terminal == TEST EXECUTION SUMMARY == (also, you can see test_log.txt for details)___
## ___Useful references___
+ ### SVG 
   + [Reference (mozilla.org)](https://developer.mozilla.org/en-US/docs/Web/SVG)
   + [W3 Schools tutorial](https://www.w3schools.com/graphics/svg_intro.asp)
   + [W3 Schools Try It editor](https://www.w3schools.com/graphics/svg_intro.asp)
+ ### [TinyXML2 library documentation.](https://leethomason.github.io/tinyxml2/)
+ ### [C++ reference (local mirror of cppreference.com)](https://moodle2324.up.pt/pluginfile.php/189580/mod_resource/content/8/en/index.html)





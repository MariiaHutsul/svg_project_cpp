# Programming project

## Group elements


* ### up202306129 - Tiago Catelas Ribeiro Correia
* ### up202310202 - Mariia Hutsul
* ### up202305501 - Martim Oliveira Neves


## Accomplished tasks

### This project concerns the images from the SVG vectorial format to the PNG raster format.
### The main task in this project is defining a hierarchy of C++ classes corresponding to SVG elements, and code for reading these elements. (  [read more](https://moodle2324.up.pt/pluginfile.php/189678/mod_resource/content/20/index.html) )  
* ### ___The code is compiled with  Makefile___
* ### ___In project directory, you can run [ $  make  ] to compile the project if everything is Ok or you can be able to see errors if something goings wrong___
   + ### ___How undersand that everything is going Ok?___
   + ### ___In terminal have to be:___
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o svgtopng.o svgtopng.cpp 
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o Color.o Color.cpp
     * g++ -std=c++11  -pedantic -Wall
     * ...
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -o xmldump xmldump.o libproj.a   
* ### ___Launches the test runner [ $ ./test  ] and you be able to see in terminal == TEST EXECUTION SUMMARY == (also, you can see test_log.txt for details)___
## ___Useful references___
+ ### SVG 
   + [Reference (mozilla.org)](https://developer.mozilla.org/en-US/docs/Web/SVG)
   + [W3 Schools tutorial](https://www.w3schools.com/graphics/svg_intro.asp)
   + [W3 Schools Try It editor](https://www.w3schools.com/graphics/svg_intro.asp)
+ ### [TinyXML2 library documentation.](https://leethomason.github.io/tinyxml2/)
+ ### [C++ reference (local mirror of cppreference.com)](https://moodle2324.up.pt/pluginfile.php/189580/mod_resource/content/8/en/index.html)





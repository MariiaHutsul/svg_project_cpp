# Programming project

## Group elements


* ### ___up202306129 - Tiago Catelas Ribeiro Correia___
* ### ___up202310202 - Mariia Hutsul___
* ### ___up202305501 - Martim Oliveira Neves___


## Accomplished tasks

### This project concerns the conversion of images from the SVG vectorial format to the PNG raster format.
### The main task in this project is defining a hierarchy of C++ classes corresponding to SVG elements, and code for reading these elements. (  [read more](https://moodle2324.up.pt/pluginfile.php/189678/mod_resource/content/20/index.html) )  
* ### The code is compiled using Makefile
* ### In the project directory, you can run [ $ make test ] to compile the project, if everything is Ok, or you will be able to see errors if something goes wrong
   + ### ___How to understand that it compiled properly?___
   + ### ___In terminal it should be:___
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o test.o test.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o Color.o Color.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o Point.o Point.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o PNGImage.o PNGImage.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o SVGElements.o SVGElements.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o readSVG.o readSVG.cpp
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -c -o convert.o convert.cpp ar cr libproj.a external/tinyxml2/tinyxml2.o Color.o Point.o PNGImage.o Point.o SVGElements.o readSVG.o convert.o 
     * g++ -std=c++11  -pedantic -Wall -Wuninitialized -Werror -g -fsanitize=address -fsanitize=undefined -o test test.o libproj.a   
* ### [ $ ./test  ] Launches the test runner and you will be able to see in terminal ==test execution summary== (also, you can see test_log.txt for details)
## ___Useful references___
+ #### SVG 
   + [Reference (mozilla.org)](https://developer.mozilla.org/en-US/docs/Web/SVG)
   + [W3 Schools tutorial](https://www.w3schools.com/graphics/svg_intro.asp)
   + [W3 Schools Try It editor](https://www.w3schools.com/graphics/svg_intro.asp)
+ #### [TinyXML2](https://leethomason.github.io/tinyxml2/)
+ #### [C++ reference ](https://moodle2324.up.pt/pluginfile.php/189580/mod_resource/content/8/en/index.html)
### The main concepts of C++ used in the project:

+ #### Object-Oriented Programming (OOP):  
   classes, inheritance, polymorphism, and encapsulation to organize and structure the SVG manipulation functionality.

+ #### Standard Template Library (STL): 
  maps (std::map), vectors (std::vector), strings (std::string), pairs (std::pair), and algorithms (std::replace, std::find, std::sort), to efficiently manage and manipulate data structures.

+ #### Input/Output Streams:  
  <iostream> header are used to read and write data, facilitating communication with the user or external files.

+ #### Memory Management: 
  memory management practices through dynamic memory allocation (new), pointer manipulation, and resource deallocation (delete) to ensure proper memory usage and avoid memory leaks.

+ #### String Manipulation: 
  such as tokenization, parsing, and substring extraction are employed using standard string manipulation functions and utilities .

+ #### Exception Handling: 
   to handle runtime errors gracefully, ensuring robustness and reliability in case of file loading failures or other exceptional conditions.

+ #### Third-Party Libraries Integration: 
  the tinyxml2 library for parsing XML files, extending the functionality of the program by leveraging external libraries.

Conclusion:
This project demonstrates a deep understanding of C++ programming concepts, including object-oriented design, efficient memory management, and robust error handling. By leveraging both the Standard Template Library and third-party libraries, the team successfully created a functional and efficient tool for converting SVG files to PNG format. The project also highlights the importance of thorough testing and debugging in ensuring software reliability and performance.





# rasterization-project

This program allows you to draw simple triangles interactively. You can insert, delete, translate, scale, rotate, and color triangles. You may color any triangle by changing the color of any of its vertices.  

The different functions are triggered based on different events. These events call their corresponding glfw callback functions, one of which is the key callback, so depending on which key the user presses, the callback function either sets a state or simply just, within the key callback function, executes what the user wants to do.

To create a triangle, the user presses the key "i" and clicks three times on any point in the window. This is when a triangle is created:

![Triangle](https://github.com/nickwyl/rasterization-project/blob/master/images/triangle.png)


## Getting Started

You must install a C++ compiler and CMake (as a build system) before you can run the program. You may install CMake with a package manager; if on Debian/Ubuntu use ```$ sudo apt-get install cmake```, if on Mac use ```brew install cmake```, if on Windows use ```choco install -y cmake```.

### Compiling the Project

To view the result, please clone or download this repository and follow the commands:

1. Navigate to the proj directory and create a directory called build:
  ```
  $ cd proj; mkdir build
  ```
2. Use CMake to generate the Makefile/project files needed for compilation inside the build/ directory:
  ```
  $ cd build; cmake -DCMAKE_BUILD_TYPE=Release ..
  ```
3. Now we compile and run the executable: 
  ```
  $ make; ./assignment5
  ```

## Acknowledgments

I worked with Professor Panozzo’s Assignment 5 starting code.

# rasterization-project

This program allows you to draw simple triangles interactively. You can insert, delete, translate, scale, rotate, and color triangles.

## Getting Started

You must install a C++ compiler and CMake (as a build system) before you can run the program. You may install CMake with a package manager; if on Debian/Ubuntu use ```$ sudo apt-get install cmake```, if on Mac use ```brew install cmake```, if on Windows use ```choco install -y cmake```.

### Compiling the Project

To view the result, please clone or download this repository and follow the commands:

1. Navigate to the rasterization-project directory and create a directory called build:
  ```
  $ mkdir build
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

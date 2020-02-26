# rasterization-project

This program allows you to draw simple triangles interactively. You can insert, delete, translate, scale, rotate, and color triangles. You may color any triangle by changing the color of any of its vertices.  

The different functions are triggered based on different events. These events call their corresponding glfw callback functions, one of which is the key callback, so depending on which key the user presses, the callback function either sets a state or simply just, within the key callback function, executes what the user wants to do.

To create a triangle, the user presses the key "i" and clicks three times on any point in the window. All triangles are contained in one VBO. The program begins with a 2x3 matrix. The first triangle populates this matrix with the coordinates of its vertices. Then, when a user wants to create another triangle, the first click resizes the matrix, adding three more columns to the matrix for the next triangle, and so on.

This is when a triangle is created:

![Triangle](https://github.com/nickwyl/rasterization-project/blob/master/images/triangle.png)

To move a triangle, the user presses the key “o” and clicks on a triangle. While the mouse is held down, the triangle will turn from whatever color it is to blue and can be dragged anywhere. When the mouse is released, it will turn back to its original color (to be more explicit, the color of the vertices of the triangle will be preserved even after dragging the triangle around).

This is when a triangle is selected:

![Triangle Selected](https://github.com/nickwyl/rasterization-project/blob/master/images/triangle-clicked.png)

To delete a triangle, the user presses the key “p” and selects a triangle. The three coordinates of the triangle are made the same, which turns into a point, so it will not be drawn.

For both translating and deleting, the point at where the user click is determined if it’s within a triangle. I use barycentric coordinates to determine this.

To scale a triangle, the user presses the key "h" to scale up and "j" to scale down. To rotate a triangle, the user presses the key "k" to rotate clockwise and "l" to rotate counter-clockwise.
The code for this occurs within the key callback function. The barycenter of the selected triangle is calculated. For scaling, the triangle will scale with respect to the barycenter (vertices will grow or shrink WRT to barycenter). Similarly, the triangle will rotate with respect to the barycenter.

The user can also color a triangle by coloring its vertices. To enable coloring mode, the user presses the key "c" and clicks the mouse. The vertex closest to the mouseclick will be selected, and the user can then color the vertex by pressing keys "1" to "9", each of which is a different color. The colors of the vertices are linearly interpolated. In the key callback function, there are nine different cases, from 1-9, for different colors. I created a matrix (named VC) and a second VBO (named VBOC) to store the attributes of the color. A “color” vector is passed in and a “vColor” vector is passed out of the vertex shader. A “vertexColor” vector is passed in and an “outColor” vector is passed out of the vertex shader. A few lines below, I connect the VBOC I defined with the position “color” in the vertex shader. The user can click anywhere to color a vertex, as the program will help find the nearest vertex of any triangle to where was clicked.

This is when a triangle is colored by the user:

![Colored Triangle](https://github.com/nickwyl/rasterization-project/blob/master/images/triangle-colored.png)


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

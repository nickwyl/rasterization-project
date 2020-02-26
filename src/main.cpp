// This example is heavily based on the tutorial at https://open.gl

////////////////////////////////////////////////////////////////////////////////
// OpenGL Helpers to reduce the clutter
#include "helpers.h"
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
// Linear Algebra Library
#include <Eigen/Dense>
// Timer
#include <chrono>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////

using namespace Eigen;


//VertexBufferObject wrapper
VertexBufferObject VBO; //Used to create a buffer to later send vertex data from CPU to graphics card
VertexBufferObject VBOC;

//State to track which mode (object creation, scaling, moving, etc.)
int state;

//Set up matrix to store vertices
Eigen::MatrixXf V(2,3); 
//Set up matrix to store the color of each vertex
Eigen::MatrixXf VC(3,3);

//Keep track of total number of vertices in the program
int numOfVertices = 0;
//Keep track of which vertex of the current triangle we're working with
int currentTriangleVertex = 0;

//Keep track of whether mouse is pressed or released
bool mousePressed = false;
bool releasedAlready = false;

int whichTrianglePressed;

//Keep track of vertices' color
int colorThisVertex = 1;
Vector3f originalColorV1;
Vector3f originalColorV2;
Vector3f originalColorV3;

Vector2f pa;
Vector2f pb;
Vector2f pc;

const float PI = 3.141592654;


//Set global variable for state
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { 
	//Record which key user presses
	if (action == GLFW_PRESS) {
		switch (key) {
			//Enable triangle insertion mode
			case GLFW_KEY_I: {
				state = 1;
				break;
			}
			//Enable triangle translation mode
			case GLFW_KEY_O: {
				state = 2;
				releasedAlready = false;
				break;
			}
			//Enable triangle deletion mode
			case GLFW_KEY_P: {
				state = 3;
				break;
			}
			//Rotate triangle 10 deg clockwise
			case GLFW_KEY_H: {
				if (state==2) {
					MatrixXf barycenter(2,3);

					Vector2f bc = (V.col(whichTrianglePressed)+V.col(whichTrianglePressed+1)+V.col(whichTrianglePressed+2))/3;
					barycenter<<bc,bc,bc;


					MatrixXf currentTriangle(2,3);
					currentTriangle<<V.col(whichTrianglePressed)(0),V.col(whichTrianglePressed+1)(0),V.col(whichTrianglePressed+2)(0),
									 V.col(whichTrianglePressed)(1),V.col(whichTrianglePressed+1)(1),V.col(whichTrianglePressed+2)(1);

					//Rotation matrix
					Matrix2f rotation;
					rotation << cos(-10*PI/180),-sin(-10*PI/180),
								sin(-10*PI/180),cos(-10*PI/180);


					MatrixXf newTriangle = rotation*(currentTriangle-barycenter)+barycenter;
					
					V.col(whichTrianglePressed) = newTriangle.col(0);
					V.col(whichTrianglePressed+1) = newTriangle.col(1);
					V.col(whichTrianglePressed+2) = newTriangle.col(2);
				}
				break;
			}
			//Rotate triangle 10 deg counter-clockwise
			case GLFW_KEY_J: {
				if (state==2) {
					MatrixXf barycenter(2,3);

					Vector2f bc = (V.col(whichTrianglePressed)+V.col(whichTrianglePressed+1)+V.col(whichTrianglePressed+2))/3;
					barycenter<<bc,bc,bc;

					MatrixXf currentTriangle(2,3);
					currentTriangle<<V.col(whichTrianglePressed)(0),V.col(whichTrianglePressed+1)(0),V.col(whichTrianglePressed+2)(0),
									 V.col(whichTrianglePressed)(1),V.col(whichTrianglePressed+1)(1),V.col(whichTrianglePressed+2)(1);

					//Rotation matrix
					Matrix2f rotation;
					rotation << cos(10*PI/180),-sin(10*PI/180),
								sin(10*PI/180),cos(10*PI/180);

					MatrixXf newTriangle = rotation*(currentTriangle-barycenter)+barycenter;

					V.col(whichTrianglePressed) = newTriangle.col(0);
					V.col(whichTrianglePressed+1) = newTriangle.col(1);
					V.col(whichTrianglePressed+2) = newTriangle.col(2);
				}
				break;
			}
			//Scale up triangle by 25%
			case GLFW_KEY_K: {
				if (state == 2) {
					//Calculate barycenter
					Vector2f barycenter = (V.col(whichTrianglePressed)+V.col(whichTrianglePressed+1)+V.col(whichTrianglePressed+2))/3;

					V.col(whichTrianglePressed) = barycenter+1.25*((V.col(whichTrianglePressed)-barycenter));
					V.col(whichTrianglePressed+1) = barycenter+1.25*((V.col(whichTrianglePressed+1)-barycenter));
					V.col(whichTrianglePressed+2) = barycenter+1.25*((V.col(whichTrianglePressed+2)-barycenter));
				}
				break;
			}
			//Scale down triangle by 25%
			case GLFW_KEY_L: {
				if (state == 2) {
					//Calculate barycenter
					Vector2f barycenter = (V.col(whichTrianglePressed)+V.col(whichTrianglePressed+1)+V.col(whichTrianglePressed+2))/3;

					V.col(whichTrianglePressed) = barycenter+0.75*((V.col(whichTrianglePressed)-barycenter));
					V.col(whichTrianglePressed+1) = barycenter+0.75*((V.col(whichTrianglePressed+1)-barycenter));
					V.col(whichTrianglePressed+2) = barycenter+0.75*((V.col(whichTrianglePressed+2)-barycenter));
				}
				break;
			}
			//Enable triangle coloring mode
			case GLFW_KEY_C: {
				state = 4;
				break;
			}
			//Below are the different colors
			case GLFW_KEY_1: {
				if (state == 4) {
					VC.col(colorThisVertex) << 0.2,0.5,0.9;
				}
				break;
			}
			case GLFW_KEY_2: {
				if (state == 4) {
					VC.col(colorThisVertex) << 0,1,0;
				}
				break;
			}
			case GLFW_KEY_3: {
				if (state == 4) {
					VC.col(colorThisVertex) << 0,0,1;					
				}
				break;
			}
			case GLFW_KEY_4: {
				if (state == 4) {
					VC.col(colorThisVertex) << 1,1,0;										
				}
				break;
			}
			case GLFW_KEY_5: {
				if (state == 4) {
					VC.col(colorThisVertex) << 1,0,1;										
				}
				break;
			}
			case GLFW_KEY_6: {
				if (state == 4) {
					VC.col(colorThisVertex) << 0,1,1;															
				}
				break;
			}
			case GLFW_KEY_7: {
				if (state == 4) {
					VC.col(colorThisVertex) << 0.6,0.1,0.3;																				
				}
				break;
			}
			case GLFW_KEY_8: {
				if (state == 4) {
					VC.col(colorThisVertex) << 0.4,0.8,0.3;																									
				}
				break;
			}
			case GLFW_KEY_9: {
				if (state == 4) {
					VC.col(colorThisVertex) << 0.4,0.2,0.7;																														
				}
				break;
			}		
			//Default mode	
			default: {
				state = 0;
				break;
			}
		}
	}

	//Upload change to the GPU
	VBO.update(V);
	VBOC.update(VC);

}


//Track the location of the cursor and populate the matrix for the current triangle accordingly
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

	//Get viewport size (canvas in number of pixels)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	//Get the size of the window (may be different than the canvas size on retina displays)
	int width_window, height_window;
	glfwGetWindowSize(window, &width_window, &height_window);

	//Deduce position of the mouse in the viewport
	double highdpi = (double) width / (double) width_window;
	xpos *= highdpi;	//Need to change xpos and ypos, look through them as if they are pixels on screen
	ypos *= highdpi;

	//Convert screen position to world coordinates
	double xworld = ((xpos/double(width))*2)-1;
	double yworld = (((height-1-ypos)/double(height))*2)-1; //Y axis is flipped in glfw

	//If in triangle creation mode, populate the matrix of the current triangle based on location of the cursor 
	if (state==1 && numOfVertices%3!=0) {
		if (numOfVertices%3==1) {
			V.col(numOfVertices) << xworld, yworld;
			V.col(numOfVertices+1) << xworld, yworld;
		}
		else {
			V.col(numOfVertices) << xworld, yworld;
		}
	}
	//If in triangle translation mode and user pressing on triangle, populate matrix based on location of triangle and cursor 
	else if (state==2 && mousePressed && !releasedAlready) {

		Vector2f cursorPoint(xworld,yworld);

		V.col(whichTrianglePressed) = cursorPoint + pa;
		V.col(whichTrianglePressed+1) = cursorPoint + pb;
		V.col(whichTrianglePressed+2) = cursorPoint+ pc;

	}

	//Upload the change to the GPU
	VBO.update(V);
}


//Record the location of the cursor when clicked
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
	// Get viewport size (canvas in number of pixels)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// Get the size of the window (may be different than the canvas size on retina displays)
	int width_window, height_window;
	glfwGetWindowSize(window, &width_window, &height_window);

	// Get the position of the mouse in the window
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Deduce position of the mouse in the viewport
	double highdpi = (double) width / (double) width_window;
	xpos *= highdpi;
	ypos *= highdpi;

	// Convert screen position to world coordinates
	double xworld = ((xpos/double(width))*2)-1;
	double yworld = (((height-1-ypos)/double(height))*2)-1; //Y axis is flipped in glfw

	//If in triangle creation mode
	if (state==1) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			//If starting to create new triangle, append three columns to the matrices
			if (numOfVertices%3==0 && numOfVertices>0) {
				V.conservativeResize(V.rows(),V.cols()+3);
				VC.conservativeResize(VC.rows(),VC.cols()+3);

				//Update the color of vertices to red
				for (int i=numOfVertices; i<numOfVertices+3; i++) {
					VC.col(i) << 1,0,0;
				}
			}

			//Update color of first triangle to red
			if (numOfVertices==0) {
				for (int i=0; i<3; i++) {
					VC.col(i) << 1,0,0;
				}
			}

			//Store position of mouse in current vertex		
			V.col(numOfVertices) << xworld, yworld;

			//Update number of vertices
			numOfVertices++;
			//Track which vertex of the current triangle we're currently working with
			currentTriangleVertex++;

			//If on the second (out of three) vertex, update the second and third vertices with position of mouse
			if (numOfVertices%3==1) {
				V.col(numOfVertices) << xworld, yworld;
				V.col(numOfVertices+1) << xworld, yworld;
			}

			//Upload the change to the GPU
			VBO.update(V);
		}
	}
	//Reset vertex of current triangle to 0 after all vertices determined
	if (currentTriangleVertex==3) {
		state = 0;
		currentTriangleVertex = 0;
	}
	//Translating the triangle
	if (state==2) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			for (int i=0; i<numOfVertices; i+=3) {

				//Three vertices (triangle) and their points
				float v1x = V.col(i)(0);
				float v1y = V.col(i)(1);	
				float v2x = V.col(i+1)(0);
				float v2y = V.col(i+1)(1);
				float v3x = V.col(i+2)(0);
				float v3y = V.col(i+2)(1);

				//Barycentric Interpolation
				//Calculate ratio of triangle area formed by cursor, vertex 2, vertex 3
				float a = ((v2y-v3y)*(xworld-v3x)+(v3x-v2x)*(yworld-v3y))/((v2y-v3y)*(v1x-v3x)+(v3x-v2x)*(v1y-v3y));
				//Calculate ratio of triangle area formed by cursor, vertex 1, vertex 3
				float b = ((v3y-v1y)*(xworld-v3x)+(v1x-v3x)*(yworld-v3y))/((v2y-v3y)*(v1x-v3x)+(v3x-v2x)*(v1y-v3y));
				//Calculate ratio of triangle area formed by cursor, vertex 1, vertex 2
				float c = 1-a-b;


				//Check if mouse clicked on triangle. If ratios of triangle areas are all between 0 and 1 (inclusive),
				//then mouse clicked on triangle
				if (a>=0 && a<=1 && b>=0 && b<=1 && c>=0 && c<=1) {
					mousePressed = true;
					whichTrianglePressed = i;

					Vector2f clickPoint(xworld,yworld);

					//Update vectors to vector formed from original vertices to clicked point (position of triangle
					//is updated in cursor_position_callback)
					pa = V.col(i)-clickPoint;
					pb = V.col(i+1)-clickPoint;
					pc = V.col(i+2)-clickPoint;

					//Keep track of color of triangle before it was clicked
					originalColorV1 = VC.col(whichTrianglePressed);
					originalColorV2 = VC.col(whichTrianglePressed+1);
					originalColorV3 = VC.col(whichTrianglePressed+2);

					//Change triangle color to blue
					VC.col(whichTrianglePressed) << 0,0,1;			 
					VC.col(whichTrianglePressed+1) << 0,0,1;			 
					VC.col(whichTrianglePressed+2) << 0,0,1;

					break;			 
				}
			}
		}
		//When user releases the mouse button
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

			mousePressed = false;
			releasedAlready = true;

			//Return triangle to its original color before it was clicked
			VC.col(whichTrianglePressed) = originalColorV1;			 
			VC.col(whichTrianglePressed+1) = originalColorV2;			 
			VC.col(whichTrianglePressed+2) = originalColorV3;
		}
	}
	//If in triangle deletion mode
	else if (state==3) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			for (int i=0; i<numOfVertices; i+=3) {

				//Three vertices (triangle) and their points
				float v1x = V.col(i)(0);
				float v1y = V.col(i)(1);	
				float v2x = V.col(i+1)(0);
				float v2y = V.col(i+1)(1);
				float v3x = V.col(i+2)(0);
				float v3y = V.col(i+2)(1);
				
				//Barycentric Interpolation
				//Calculate ratio of triangle area formed by cursor, vertex 2, vertex 3
				float a = ((v2y-v3y)*(xworld-v3x)+(v3x-v2x)*(yworld-v3y))/((v2y-v3y)*(v1x-v3x)+(v3x-v2x)*(v1y-v3y));
				//Calculate ratio of triangle area formed by cursor, vertex 1, vertex 3
				float b = ((v3y-v1y)*(xworld-v3x)+(v1x-v3x)*(yworld-v3y))/((v2y-v3y)*(v1x-v3x)+(v3x-v2x)*(v1y-v3y));
				//Calculate ratio of triangle area formed by cursor, vertex 1, vertex 2
				float c = 1-a-b;


				//Check if mouse clicked on triangle. If ratios of triangle areas are all between 0 and 1 (inclusive),
				//then mouse clicked on triangle
				if (a>=0 && a<=1 && b>=0 && b<=1 && c>=0 && c<=1) {
					whichTrianglePressed = i;

					//Populate matrix for triangle to zeros (which is a point at the origin and thus "erases" the triangle)
					V.col(i) << 0,0;
					V.col(i+1) << 0,0;
					V.col(i+2) << 0,0;

					break;
				}
			}
		}
	}
	//If in triangle coloring mode
	else if (state==4) {		
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

			Vector2f clickPoint(xworld,yworld);
			float shortestDistance = (clickPoint-V.col(0)).norm();
			colorThisVertex = 0;

			//Find which triangle's vertex to color by calculating the shortest mouse click to vertex distance
			for (int i=1; i<numOfVertices; i++) {
				if ((clickPoint-V.col(i)).norm()<shortestDistance) {
					shortestDistance = (clickPoint-V.col(i)).norm();
					colorThisVertex = i;
				}
			}
		}
	}

	//Upload change to the GPU
	VBO.update(V);
	VBOC.update(VC);
}


int main(void) {
	// Initialize the GLFW library
	if (!glfwInit()) {
		return -1;
	}

	//Activate supersampling
	glfwWindowHint(GLFW_SAMPLES, 8);

	//Ensure that we get at least a 3.2 context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	//On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Create a windowed mode window and its OpenGL context
	GLFWwindow * window = glfwCreateWindow(640, 480, "[Float] Hello World", NULL, NULL); //specify resolution
	if (!window) {
		glfwTerminate();
		return -1;
	}

	//Make the window's context current
	glfwMakeContextCurrent(window);

	//Load OpenGL and its extensions
	if (!gladLoadGL()) {
		printf("Failed to load OpenGL and its extensions");
		return(-1);
	}
	printf("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);

	int major, minor, rev;
	major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
	printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
	printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	//By here, we are done opening a window


	//Initialize the VAO
	//A Vertex Array Object (or VAO) is an object that describes how the vertex
	//attributes are stored in a Vertex Buffer Object (or VBO). This means that
	//the VAO is not the actual object storing the vertex data,
	//but the descriptor of the vertex data.
	VertexArrayObject VAO;
	VAO.init();
	VAO.bind();

	//Initialize the VBO with the vertices data
	//A VBO is a data container that lives in the GPU memory
	VBO.init();
	VBOC.init();

	V.resize(2,3); 
	VC.resize(3,3); 
	
	VBO.update(V);
	VBOC.update(VC);

	//Initialize the OpenGL Program
	//A program controls the OpenGL pipeline and it must contains
	//at least a vertex shader and a fragment shader to be valid
	Program program;

	//Vertex Shader
	const GLchar* vertex_shader = R"(
		#version 150 core

		in vec2 position;
		in vec3 color;

		out vec3 vertexColor;

		void main() {
			vertexColor = color;
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)";
	//Fragment Shader
	const GLchar* fragment_shader = R"(
		#version 150 core

		in vec3 vertexColor;

		out vec4 outColor;

		void main() {
		    outColor = vec4(vertexColor, 1.0);
		}
	)";

	//Compile the two shaders and upload the binary to the GPU
	//Note that we have to explicitly specify that the output "slot" called outColor
	//is the one that we want in the fragment buffer (and thus on screen)
	program.init(vertex_shader, fragment_shader, "outColor");
	program.bind();

	//The vertex shader wants the position of the vertices as an input.
	//The following line connects the VBO we defined above with the position "slot"
	//in the vertex shader
	program.bindVertexAttribArray("position", VBO);
	program.bindVertexAttribArray("color", VBOC);


	//Register the keyboard callback
	glfwSetKeyCallback(window, key_callback);

	//Register the mouse callback
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Register the cursor position callback
	glfwSetCursorPosCallback(window, cursor_position_callback);


	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {

		//Set the size of the viewport (canvas) to the size of the application window (framebuffer)
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//Bind VAO (not necessary if you have only one)
		VAO.bind();

		//Bind program
		program.bind();


		//Clear the framebuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw outline of line or triangle
		glDrawArrays(GL_TRIANGLES, 0, numOfVertices-(numOfVertices%3));

		//If in triangle creation mode
		if (state==1) {
			//Draw line if there is one vertex
			if (numOfVertices%3==1) {
				glDrawArrays(GL_LINES, 3*((numOfVertices-1)/3), 3);
			}
			//Draw line loop if there are two vertices
			else if (numOfVertices%3==2)
				glDrawArrays(GL_LINE_LOOP, 3*((numOfVertices-1)/3), 3);
		}

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();
	}

	//Deallocate OpenGL memory
	program.free();
	VAO.free();
	VBO.free();

	//Deallocate glfw internals
	glfwTerminate();
	return 0;
}
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

static void GLClearError();
static void GLCheckError();

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Forward compat for macOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

	GLuint WINDOW_WIDTH = 800;
	GLuint WINDOW_HEIGHT = 600;

	// Initialize the window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the current window the context
	glfwMakeContextCurrent(window);

	// Initialie glad after makng a window
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Print GL Version
	std::cout << glGetString(GL_VERSION) << std::endl;

	// issues on retina display such that the width and height of the viewport
	// must be the same as the frame buffer size
	// https://stackoverflow.com/questions/31303291/opengl-rendering-constricted-to-bottom-left-quarter-of-the-screen
	GLint frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	// Set the size of the window view port
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	// Resize the viewport when the user changes the size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// Vertices to draw an equilateral triangle
	GLfloat vertices[] = 
	{	// positions        // Colours
		-0.5f,  -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,	 // 0 - bottom right
		 0.5f,  -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,   // 1 - bottom left
		 0.0f,   0.5f,  0.0f,  0.0f,  0.0f,  1.0f	 // 2 - top middle
	};

	unsigned int indices[] =
	{
		0, 1, 2
	};

	// Vertices and indices to draw 3 stacking triangles on top of each other
	GLfloat three_triangle_vert[] =
	{
		-0.5f, -0.5f, 0.0f, 	// 0 - bottom left
		 0.0f, -0.5f, 0.0f,		// 1 - bottom bottom
		 0.5f, -0.5f, 0.0f,		// 2 - bottom right
		-0.25f, 0.0f, 0.0f,		// 3 - middle left
		 0.25f, 0.0f, 0.0f,		// 4 - middle right
		 0.0f,	0.5f, 0.0f		// 5 - top
	};

	unsigned int three_triangle_indices[] =
	{
		0, 1, 3,
		1, 2, 4,
		3, 4, 5
	};

	// Create a VBO, VAO, EBO
	// VAO = Vertex Attribute Object
	// VBO = Vertex Buffer Object
	// EBO = Element Buffer Object
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	// Generate Buffers for VBO and EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);

	// 2. copy our vertices array in a buffer for OpenGL to use
	//  Creates a data store for the buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, VBO);			// Binding VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// copy index array in an element buffer for OpenGL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Binding EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 3. then set our vertex attributes pointers

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
	(void*)0);
	glEnableVertexAttribArray(0); // Enable position attribute

	// Colour Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);	// Enabled colour attribute

	// Create a Shader Program from a vertex shader source and fragment shader source
	const char *vertexShaderSource = "include/shaders/VertShader.glsl";
	const char *fragmentShaderSource = "include/shaders/FragShader.glsl";

	// Shader Program Constur
	Shader ourShader(vertexShaderSource, fragmentShaderSource);
	
	// Wireframe mode if enabled
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render Loop
	while(!glfwWindowShouldClose(window))
	{
		// Handle input
		processInput(window);

		// ... Render here
		// Clear the colourbufer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the Shader Program
		// glUseProgram(shaderProgram);
		ourShader.use();

		// update the uniform color
		// float timeValue = glfwGetTime();
		// float greenValue = std::sin(timeValue) / 2.0f + 0.5f;
		// int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColour");
		// glUniform4f(vertexColorLocation, 0.05f, greenValue, 0.05f, 1.0f);

		// Drawing code in render loop
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(VAO);
		GLClearError();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		GLCheckError();

		// Unbind the VAO
		glBindVertexArray(0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// terminiate glfw
	glfwTerminate();
	return 0;
}

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{
		
	}
}

static void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (0x" << std::hex << error << ")"  << std::endl;
	}
}

// Resizes the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process Keyboard input
void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);
}
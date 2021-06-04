#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{	
	if (!glfwInit())
	{
		std::cout << "You suck dude, get GLFW to work!\n";
	}

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Set error callback
	glfwSetErrorCallback(error_callback);

	// Initialise a window
	GLFWwindow* window = glfwCreateWindow(1280, 960, "Finally wowowow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	// Make GL context current. 
	// GLFW doesn't leave the GL context current when glfwCreateWindow() succeeds.
	// https://stackoverflow.com/questions/48650497/glad-failing-to-initialize/48650985
	glfwMakeContextCurrent(window);

	// Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD\n";
		return -1;
	}

	// Get the current GLFW version
	std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;

	// Set the size of the rendering window
	glViewport(0, 0, 1280, 960);


	float t[] =
	{
		-.5f, -.5f, 0.f,
		0.f, .5f, 0.f,
		.5f, -.5f, 0.f
	};
	uint32_t v;
	glGenBuffers(1, &v);
	glBindBuffer(GL_ARRAY_BUFFER, v);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]),
		&t[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3,
		 (void*)0);
	glEnableVertexAttribArray(0);

	while(!glfwWindowShouldClose(window))
	{	
		// Input
		processInput(window);

		// Clear the screen from the previous pane
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Resize the window
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}


void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %s\n", description);
}

// Fill in the proper argument for changing window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Closes the window if the user press the ESC key
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
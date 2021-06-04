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


	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	// Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	// Simple Vertex Shader using GLSL
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//uint32_t v;
	//glGenBuffers(1, &v);
	//glBindBuffer(GL_ARRAY_BUFFER, v);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]),&t[0], GL_STATIC_DRAW);

	// Check for errors when compiling the vertex shader
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// Print error message for any compilation errors for the vertex shader
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Simple Fragment Shader with orange colour
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for errors when compiling the fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	// Print error message for compilation errors for the fragment shader
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for errors when compiling the shader program
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgram);

	// Delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Process to generate a VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	// 3. set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3,(void*)0);
	//glEnableVertexAttribArray(0);

	while(!glfwWindowShouldClose(window))
	{	
		// Input
		processInput(window);

		// Clear the screen from the previous pane
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the triangle using the shaderProgram and VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
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
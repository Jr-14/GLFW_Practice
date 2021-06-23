#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sstream>
#include <fstream>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

static std::string ParseShader(const std::string &filepath);
static unsigned int CompileShader(unsigned int type, const std::string &source);
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

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


	// Vertices to draw
	GLfloat vertices[] = 
	{
		-0.5f, -0.5f, 0.0f, // 0
		0.5f, -0.5f, 0.0f,  // 1
		0.5f, 0.5f, 0.0f,	// 2
		-0.5f, 0.5f, 0.0f,	// 3
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
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
	glBindBuffer(GL_ARRAY_BUFFER, VBO);			// Binding VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Binding EBO

	//  Creates a data store for the buffer objects
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
	(void*)0);
	glEnableVertexAttribArray(0);

	// Create a Shader Program from a vertex shader source and fragment shader source
	std::string vertexShaderSource = ParseShader("include/shaders/VertShader.glsl");
	std::string fragmentShaderSource = ParseShader("include/shaders/FragShader.glsl");
	GLuint shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);
	// Use the Shader Program
	glUseProgram(shaderProgram);
	
	// Wireframe mode if enabled
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	// Render Loop
	while(!glfwWindowShouldClose(window))
	{
		// Handle input
		processInput(window);

		// ... Render here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing code in render loop
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		GLClearError();
		glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
		GLCheckError();

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

// Read the source code and parse the string
static std::string ParseShader(const std::string &filepath)
{
	std::ifstream stream(filepath);
	std::stringstream ss;
	std::string line;
	while (getline(stream, line))
	{			
		ss << line << '\n';
	}
	return ss.str();
}

// Compile the shader from a source file
static unsigned int CompileShader(unsigned int type, const std::string &shaderSource)
{	
	// Create the Shader
	unsigned int id = glCreateShader(type);
	const char* src = shaderSource.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	// Check if Compilation is successful
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

// Create the shader from a compiled program
static unsigned int CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{	
	// Create Shader Program
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// Attach the Vertex and Fragment Shader into the program
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	// Logging for any errors with
	GLchar infoLog[512];
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	// Delete the Shaders once the program has been created
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return shaderProgram;
}

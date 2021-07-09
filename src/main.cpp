#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

// OpenGL Mathmematics (glm)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Forward Compatibility with Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// std::cout << "Hello World!! FUCK" << std::endl;


	// Create a window!
	GLFWwindow* window = glfwCreateWindow(1200, 900, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread. 
	// A context must only be made current on a single thread at a time and each thread can have only a single 
	// current context at a time.
	glfwMakeContextCurrent(window);

	// See if we can put transformation by translating a vector of (1,0,0) by (1,1,0)
	// Note that we define it as glm::vec4 with its homogenous coordinate set 1.0:
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;

	// Make Context current first before initialising glad
	// https://stackoverflow.com/questions/48650497/glad-failing-to-initialize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Render the window
	glViewport(0, 0, 1200, 900);

	// tell GLFW we want to call this function on every window resize by registering it
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// int nrAttributes;
	// glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	// std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	const char *vertexShaderSource = "shaderSource/VertShader.glsl";
	const char *fragmentShaderSource = "shaderSource/FragShader.glsl";
	// Use Create and Compile, and Create a Shader Program
	Shader ourShader(vertexShaderSource, fragmentShaderSource);

	// vertices of our triangle in NDC
	float vertices[] = 
	{	
		// positions        // colours        // texture coords
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
		0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
	   -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
	   -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
	};

	// indices of vertex to draw a triangle
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		2, 3, 0  // second triangle
	};

	// Vertex Buffer Objects (VBO)
	// Vertex Array Objects (VAO)
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);	// Generate VAO object
	glBindVertexArray(VAO);	// Bind Vertex Array Object
	glGenBuffers(1, &VBO);	// Generate VBO object
	glGenBuffers(1, &EBO); 	// Generate EBO object
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// Bind the EBO to an Element Array Buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copies the defined vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copies the defined vertex data into the buffer's memory

	// Tell how OpenGL should interpret the vertex data (per vertex attribute)
	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Colour Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/*Usually when you haave multiple objects you want to draw, you first generate/configure all the VAOs
	(and thus the required VBO and attribute pointers) and store those for later use. The momenet we want
	to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the
	VAO again
	*/ 

	// whole process of generating a texture 
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height,
	&nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	// Wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render Loop
	while(!glfwWindowShouldClose(window))
	{		
		// Process user keyboard input
		processInput(window);

		// Rendering commands here
		// Clear the colorbuffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader program
		ourShader.use();

		// Determine the colour of the triangle
		// float timeValue = glfwGetTime();
		// float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
		// printf("Green colour value : %1.1f\n", greenValue);
		// // std::cout << "Green Colour Value: %1.1f" <<
		// int vertexColourLocation = glGetUniformLocation(shaderProgram, "ourColour");
		// glUniform4f(vertexColourLocation, 0.0f, greenValue, 0.0f, 1.0f);	// change the green colour value

		// Render the an orange triangle
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminate GLFW
	glfwTerminate();

	return 0;
}

// Resize the viewport when user resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process keyboard input
void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);	// close the window when user presses escape
	}
}
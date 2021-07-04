#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Forward Compatibility with Mac OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	std::cout << "Hello Wolrd!! is soooo old" << std::endl;


	// Create a window!
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Make Context current first before initialising glad
	https://stackoverflow.com/questions/48650497/glad-failing-to-initialize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Render the window
	glViewport(0, 0, 800, 600);

	// tell GLFW we want to call this function on every window resize by registering it
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// Vertex Shader Source
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// Fragment Shader Source
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	// Yellow Fragment Shader Source
	const char *yellowFragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\0";
	
	// Create a vertex shader and link it to its unique ID
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // add a shader source
	glCompileShader(vertexShader); 	// compile the shader

	// Compile error variables
	int success;
	char infoLog[512];

	// Checking for compile errors within the fragment shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	// Create a fragment shader and link it to its unique ID
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checking for compile errors within the fragment Shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	// Create a yellow fragment shader and link it to its unique ID
	unsigned int yellowFragmentShader;
	yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
	glCompileShader(yellowFragmentShader);

	// Checking for compile errors within the yellow fragment Shader
	glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::yellow-FRAGMENT::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	// Create a shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checking for compile errors within the shader program
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	unsigned int yellowShaderProgram;
	yellowShaderProgram = glCreateProgram();
	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);
	glLinkProgram(yellowShaderProgram);

	// Checking for compile errors within the yellow shader program
	glGetShaderiv(yellowShaderProgram, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(yellowShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::yellow-SHADER::PROGRAM::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	// Use the shader program
	// Every shader and rendering call after glUseProgram will now use 
	// this program object (and thus the shader)
	glUseProgram(shaderProgram);

	// Delete the shader objects once linked to the program
	// Not needed anymore - say bye bye :(
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float leftTriangleVertices[] =
	{
		-0.5f,  -0.5f,  0.0f,
		 0.0f,  -0.5f,  0.0f,
		-0.25f,  0.0f,  0.0f,
	};

	float rightTriangleVertices[] =
	{
		 0.0f,  -0.5f,  0.0f,
		 0.5f,  -0.5f,  0.0f,
		 0.25f,  0.0f,  0.0f
	};

	// indices of vertex to draw 2 triangles to form a rectangle
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};

	unsigned int leftTriangleVAO, rightTriangleVAO; // Vertex Array Objects (VAO) for left and right triangles
	unsigned int leftTriangleVBO, rightTriangleVBO; // Vertex Buffer Objects (VBO) for left and right triangles
	// Generate Vertex Arrays for left and right triangles
	glGenVertexArrays(1, &leftTriangleVAO);
	glGenVertexArrays(1, &rightTriangleVAO);

	glGenBuffers(1, &leftTriangleVBO);	// Generate VBO object for left triangle
	glGenBuffers(1, &rightTriangleVBO); // Generate VBO object for left triangle

	// Configure the left triangle
	// Bind Vertex Arrays for the left triangle
	glBindVertexArray(leftTriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, leftTriangleVBO);		// Bind the left Triangle VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangleVertices), leftTriangleVertices, GL_STATIC_DRAW);
	// Tell how OpenGL should interpret the vertex data (per vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Configure the right triangle
	// Bind Vertex Arrays for the right triangle
	glBindVertexArray(rightTriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rightTriangleVBO);	// Bind the right Triangle VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangleVertices), rightTriangleVertices, GL_STATIC_DRAW);
	// Tell how OpenGL should interpret the vertex data (per vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unsigned int VBO, VAO, EBO;
	// glGenVertexArrays(1, &VAO);	// Generate VAO object
    // glBindVertexArray(VAO);	// Bind Vertex Array Object
    // glGenBuffers(1, &VBO);	// Generate VBO object
    // glGenBuffers(1, &EBO); 	// Generate EBO object
    // glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the VBO
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// Bind the EBO to an Element Array Buffer
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copies the defined vertex data into the buffer's memory
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copies the defined vertex data into the buffer's memory
	// glBufferData(GL_ARRAY_BUFFER, sizeof(two_triangles), two_triangles, GL_STATIC_DRAW); // Copies the defined vertex data into the buffer's memory


	/*Usually when you haave multiple objects you want to draw, you first generate/configure all the VAOs
	(and thus the required VBO and attribute pointers) and store those for later use. The momenet we want
	to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the
	VAO again
	*/ 

	// Wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render Loop
	while(!glfwWindowShouldClose(window))
	{		
		// Process user keyboard input
		processInput(window);

		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the an orange triangle
		glUseProgram(yellowShaderProgram);

		// Render Left Triangle
		glBindVertexArray(leftTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Render using a yellow shader
		glUseProgram(shaderProgram);

		// Render Right Triangle
		glBindVertexArray(rightTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0 ,3);
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
		glfwSetWindowShouldClose(window, true);	// close the window when user presses escape
}
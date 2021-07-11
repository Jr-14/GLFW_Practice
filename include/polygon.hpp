#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.hpp"

// OpenGL Mathmematics (glm)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Polygon {
private:	
public:
	/**
	 * Constructor for creating a 3D Polygon
	 * 
	 * @param numVert	Number of Vertices within the polygon. numberOfVertices cannot be
	 * a negative number and the minimum number of vertices is 3.
	 */
	Polygon(unsigned int numVert);

	/**
	 * Destructor for a 3D Polygon
	 */
	~Polygon();

protected:
	// Reference to the VAO (Vertex Array Object)
	unsigned int VAO;

	// Reference to the VBO (Vertex Buffer Object)
	unsigned int VBO;

	// Reference to the EBO (Element Buffer Object)
	unsigned int EBO;

	// Number of Vertices within the polygon
	unsigned int numberOfVert;
};
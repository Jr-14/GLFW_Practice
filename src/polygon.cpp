#include "polygon.hpp"

Polygon::Polygon(unsigned int numVert) 
{
	glGenVertexArrays(1, &VAO);	// Generate VAO object
	glBindVertexArray(VAO);	// Bind Vertex Array Object
	glGenBuffers(1, &VBO);	// Generate VBO object
	glGenBuffers(1, &EBO); 	// Generate EBO object
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	// Bind the EBO to an Element Array Buffer

	// Number of Vertices for a polygon has to be greater than or equal to 3
	if (numberOfVert >= 3) 
	{
		this->numberOfVert = numVert;
	}
	else
	{
		throw std::invalid_argument("Number of vertices cannot be less than 3!");
	}
}

Polygon::~Polygon()
{

}
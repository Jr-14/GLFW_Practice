# version 330 core 
out vec4 FragColor;

in vec4 vertexColour;	// input variable from vertex shader (same name and type)
void main()
{
	FragColor = vertexColour;
}
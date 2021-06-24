# version 330 core 
out vec4 FragColor;

in vec3 ourColour;	// input of colour from the vertex shader

void main()
{
	FragColor = vec4(ourColour, 1.0);
}
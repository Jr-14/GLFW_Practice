# version 330 core 
out vec4 FragColor;

in vec3 ourColour;	// input of colour from the vertex shader

uniform float ourAlpha; // uniform value for the alpha

void main()
{
	FragColor = vec4(ourColour, ourAlpha);
}
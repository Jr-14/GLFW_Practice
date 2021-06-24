# version 330 core 
out vec4 FragColor;

uniform vec4 ourColour;	// we set this variable in the OpenGL code.
void main()
{
	FragColor = ourColour;
}
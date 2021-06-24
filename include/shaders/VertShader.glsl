# version 330 core 
layout (location = 0) in vec3 aPos; 

out vec4 vertexColour;	// specify a color output to the fragment shader


void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

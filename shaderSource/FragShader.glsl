#version 330 core
out vec4 FragColor;

in vec3 ourColour;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	// FragColor = vec4(ourColour, 1.0);
	// FragColor = texture(ourTexture, TexCoord);
	FragColor = mix(texture(texture1, TexCoord),
				    texture(texture2, TexCoord), 0.2);
}
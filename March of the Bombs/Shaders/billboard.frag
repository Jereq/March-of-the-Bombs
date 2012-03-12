#version 150

in vec2 texCoord;

uniform sampler2D image;

out vec4 fragColor;

void main()
{
	fragColor = texture(image, texCoord);
}
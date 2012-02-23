#version 150

in VertexData
{
	vec2 textureCoordinates;
} fragmentIn;

uniform sampler2D image;

out vec4 fragColor;

void main()
{
	fragColor = texture(image, fragmentIn.textureCoordinates);
}
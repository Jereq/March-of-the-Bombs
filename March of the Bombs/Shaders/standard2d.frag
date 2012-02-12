#version 150

in VertexData
{
	vec2 textureCoordinates;
} fragmentIn;

uniform sampler2D texture;

out vec4 fragColor;

void main()
{
	fragColor = texture(texture, fragmentIn.textureCoordinates);
}
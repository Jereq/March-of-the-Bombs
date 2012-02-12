#version 150

in VertexData
{
	vec2 textureCoordinates;
} fragmentIn;

uniform sampler2D texture;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = texture(texture, fragmentIn.textureCoordinates);
}
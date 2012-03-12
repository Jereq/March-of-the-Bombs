#version 150

in vec3 centerPosition;
in vec2 halfSize;
in vec2 textureLowerLeft;
in vec2 textureUpperRight;

uniform mat4 viewMatrix;

out VertexData
{
	vec2 halfSize;
	vec2 textureLowerLeft;
	vec2 textureUpperRight;
} vertexOut;

void main()
{
	vertexOut.halfSize = halfSize;
	vertexOut.textureLowerLeft = textureLowerLeft;
	vertexOut.textureUpperRight = textureUpperRight;

	gl_Position = viewMatrix * vec4(centerPosition, 1);
}
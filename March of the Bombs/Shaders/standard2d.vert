#version 150

in vec3 vertexPosition;
in vec2 textureCoordinates;

out VertexData
{
	vec2 textureCoordinates;
} vertexOut;

void main()
{
	vertexOut.textureCoordinates = textureCoordinates;
	gl_Position = vec4(vertexPosition, 1);
}
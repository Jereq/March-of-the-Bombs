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
	gl_Position = vec4(vertexPosition.x * 2 - 1, vertexPosition.y * 2 - 1, vertexPosition.z, 1);
}
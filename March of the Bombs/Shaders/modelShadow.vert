#version 150

in vec3 vertexPosition;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1);
}
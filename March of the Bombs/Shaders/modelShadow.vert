#version 150

in vec3 vertexPosition;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertexPosition, 1);
}
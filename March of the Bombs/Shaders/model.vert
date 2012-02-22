#version 150

in vec3 vertexPosition;
in vec2 vertexTextureCoordinates;
in vec3 vertexNormal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform mat4 shadowMatrices[5];
uniform uint numLights = 0;

out VertexOut
{
	vec4 position;
	vec2 textureCoordinates;
	vec3 normal;

	vec4 shadowCoordinates[5];
} vOut;

void main()
{
	vec4 eyeSpacePosition = modelViewMatrix * vec4(vertexPosition, 1);

	vOut.position = eyeSpacePosition;
	vOut.textureCoordinates = vertexTextureCoordinates;
	vOut.normal = normalize(normalMatrix * vertexNormal);

	for (int i = 0; i < numLights; i++)
	{
		vOut.shadowCoordinates[i] = shadowMatrices[i] * vec4(vertexPosition, 1);
	}

	gl_Position = projectionMatrix * eyeSpacePosition;
}
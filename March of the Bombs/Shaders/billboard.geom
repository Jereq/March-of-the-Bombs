#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in VertexData
{
	vec2 halfSize;
	vec2 textureLowerLeft;
	vec2 textureUpperRight;
} vertexIn[1];

out vec2 texCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	vec4 center = gl_in[0].gl_Position;
	vec2 halfSize = vertexIn[0].halfSize;

	float left = vertexIn[0].textureLowerLeft.x;
	float right = vertexIn[0].textureUpperRight.x;
	float bottom = vertexIn[0].textureLowerLeft.y;
	float top = vertexIn[0].textureUpperRight.y;

	gl_Position = projectionMatrix * vec4(center.x - halfSize.x, center.y - halfSize.y, center.z, center.w);
	texCoord = vec2(left, bottom);
	
	EmitVertex();
	
	gl_Position = projectionMatrix * vec4(center.x + halfSize.x, center.y - halfSize.y, center.z, center.w);
	texCoord = vec2(right, bottom);
	
	EmitVertex();
	
	gl_Position = projectionMatrix * vec4(center.x - halfSize.x, center.y + halfSize.y, center.z, center.w);
	texCoord = vec2(left, top);
	
	EmitVertex();
	
	gl_Position = projectionMatrix * vec4(center.x + halfSize.x, center.y + halfSize.y, center.z, center.w);
	texCoord = vec2(right, top);
	
	EmitVertex();

	EndPrimitive();
}
#version 150

in VertexOut
{
	vec4 position;
	vec2 textureCoordinates;
	vec3 normal;

	vec4 shadowCoordinates;
} fragIn;

uniform struct Light
{
	vec4 position;
	vec3 intensity;
} light;

uniform sampler2DShadow shadowMap;

uniform vec4 tint = vec4(0, 0, 0, 0);

uniform struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

uniform sampler2D diffuseMap;

out vec4 fragColor;

vec3 phongModelDiffAndSpec(in vec3 normal, in vec3 view, in vec3 diffColor)
{
	vec3 s = normalize(vec3(light.position - fragIn.position));
	vec3 r = reflect(-s, normal);

	vec3 diffuseReflectivity = material.diffuse * diffColor;

	return light.intensity *
		(diffuseReflectivity * max(dot(s, normal), 0) +
		material.specular * pow(max(dot(r, view), 0), material.shininess));
}

void main()
{
	vec3 normal = normalize(fragIn.normal);
	vec3 view = normalize(vec3(-fragIn.position));
	vec3 diffColor = vec3(texture(diffuseMap, fragIn.textureCoordinates));

	float sum = 0;
	sum += textureProjOffset(shadowMap, fragIn.shadowCoordinates, ivec2(-1, -1));
	sum += textureProjOffset(shadowMap, fragIn.shadowCoordinates, ivec2(-1,  1));
	sum += textureProjOffset(shadowMap, fragIn.shadowCoordinates, ivec2( 1, -1));
	sum += textureProjOffset(shadowMap, fragIn.shadowCoordinates, ivec2( 1,  1));

	float shadow = sum / 4;
		
	vec3 diffAndSpec = phongModelDiffAndSpec(normal, view, diffColor);
	vec3 result = shadow * diffAndSpec;

	result = mix(result, tint.xyz, tint.w);

	fragColor = vec4(result + material.ambient, 1.0);
}
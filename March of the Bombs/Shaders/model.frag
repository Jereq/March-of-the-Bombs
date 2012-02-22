#version 150

in VertexOut
{
	vec4 position;
	vec2 textureCoordinates;
	vec3 normal;

	vec4 shadowCoordinates[5];
} fragIn;

struct Light
{
	vec4 position;
	vec3 intensity;

	sampler2DShadow shadowMap;
};

uniform Light lights[5];
uniform uint numLights = 0;

uniform struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	sampler2D diffuseMap;
} material;

layout(location = 0) out vec4 fragColor;

vec3 phongModelDiffAndSpec(in Light light, in vec3 normal, in vec3 view, in vec3 diffColor)
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
	vec3 diffColor = vec3(1);//vec3(texture(material.diffuseMap, fragIn.textureCoordinates));

	vec3 result = vec3(0);

	if (numLights > 0)
	{
		float sum = 0;
		sum += textureProjOffset(lights[0].shadowMap, fragIn.shadowCoordinates[0], ivec2(-1, -1));
		sum += textureProjOffset(lights[0].shadowMap, fragIn.shadowCoordinates[0], ivec2(-1,  1));
		sum += textureProjOffset(lights[0].shadowMap, fragIn.shadowCoordinates[0], ivec2( 1, -1));
		sum += textureProjOffset(lights[0].shadowMap, fragIn.shadowCoordinates[0], ivec2( 1,  1));

		float shadow = sum / 4;
		
		vec3 diffAndSpec = phongModelDiffAndSpec(lights[0], normal, view, diffColor);
		result += shadow * diffAndSpec;
	}

	if (numLights > 1)
	{
		float sum = 0;
		sum += textureProjOffset(lights[1].shadowMap, fragIn.shadowCoordinates[1], ivec2(-1, -1));
		sum += textureProjOffset(lights[1].shadowMap, fragIn.shadowCoordinates[1], ivec2(-1,  1));
		sum += textureProjOffset(lights[1].shadowMap, fragIn.shadowCoordinates[1], ivec2( 1, -1));
		sum += textureProjOffset(lights[1].shadowMap, fragIn.shadowCoordinates[1], ivec2( 1,  1));
		
		float shadow = sum / 4;
		
		vec3 diffAndSpec = phongModelDiffAndSpec(lights[1], normal, view, diffColor);
		result += shadow * diffAndSpec;
	}

	if (numLights > 2)
	{
		float sum = 0;
		sum += textureProjOffset(lights[2].shadowMap, fragIn.shadowCoordinates[2], ivec2(-1, -1));
		sum += textureProjOffset(lights[2].shadowMap, fragIn.shadowCoordinates[2], ivec2(-1,  1));
		sum += textureProjOffset(lights[2].shadowMap, fragIn.shadowCoordinates[2], ivec2( 1, -1));
		sum += textureProjOffset(lights[2].shadowMap, fragIn.shadowCoordinates[2], ivec2( 1,  1));
		
		float shadow = sum / 4;
		
		vec3 diffAndSpec = phongModelDiffAndSpec(lights[2], normal, view, diffColor);
		result += shadow * diffAndSpec;
	}

	if (numLights > 3)
	{
		float sum = 0;
		sum += textureProjOffset(lights[3].shadowMap, fragIn.shadowCoordinates[3], ivec2(-1, -1));
		sum += textureProjOffset(lights[3].shadowMap, fragIn.shadowCoordinates[3], ivec2(-1,  1));
		sum += textureProjOffset(lights[3].shadowMap, fragIn.shadowCoordinates[3], ivec2( 1, -1));
		sum += textureProjOffset(lights[3].shadowMap, fragIn.shadowCoordinates[3], ivec2( 1,  1));
		
		float shadow = sum / 4;
		
		vec3 diffAndSpec = phongModelDiffAndSpec(lights[3], normal, view, diffColor);
		result += shadow * diffAndSpec;
	}

	if (numLights > 4)
	{
		float sum = 0;
		sum += textureProjOffset(lights[4].shadowMap, fragIn.shadowCoordinates[4], ivec2(-1, -1));
		sum += textureProjOffset(lights[4].shadowMap, fragIn.shadowCoordinates[4], ivec2(-1,  1));
		sum += textureProjOffset(lights[4].shadowMap, fragIn.shadowCoordinates[4], ivec2( 1, -1));
		sum += textureProjOffset(lights[4].shadowMap, fragIn.shadowCoordinates[4], ivec2( 1,  1));
		
		float shadow = sum / 4;
		
		vec3 diffAndSpec = phongModelDiffAndSpec(lights[4], normal, view, diffColor);
		result += shadow * diffAndSpec;
	}

	if (numLights == 0)
	{
		result = diffColor * material.diffuse + material.specular;
	}

	fragColor = vec4(result + material.ambient, 1.0);
}
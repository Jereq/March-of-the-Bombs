#include "Model.h"

#include <boost/foreach.hpp>

#include <glm/gtc/matrix_transform.hpp>
using glm::translate;
using glm::rotate;

#include <glm/gtc/matrix_inverse.hpp>
using glm::inverseTranspose;

using glm::vec3;
using glm::mat3;
using glm::mat4;

#include "Graphics.h"

std::map<std::string, Model3DS::ptr> Model::modelMap;
GLSLProgram Model::shadeProg;
GLSLProgram Model::shadowProg;

Model::Model(std::string const& fileName)
	: validModelMatrix(false), scale(1.f)
{
	if (modelMap.count(fileName) == 0)
	{
		modelMap[fileName] = Model3DS::ptr(new Model3DS(fileName));
	}

	modelData = modelMap[fileName];

	textures.push_back(GLTexture::getTexture(L"Images/skull.tga"));
	modelData->groups[0].material.texture1_map.user_ptr = (GLvoid*) textures[0]->getHandle();

	loadShadeProg();
	loadShadowProg();
}

void Model::loadShadeProg()
{
	if (shadeProg.isLinked())
	{
		return;
	}

	if (!shadeProg.compileShaderFromFile("Shaders/model.vert", GLSLShader::VERTEX))
	{
		printf("Model vertex shader failed to compile!\n%s\n", shadeProg.log().c_str());
		exit(EXIT_FAILURE);
	}
	if (!shadeProg.compileShaderFromFile("Shaders/model.frag", GLSLShader::FRAGMENT))
	{
		printf("Model fragment shader failed to compile!\n%s\n", shadeProg.log().c_str());
		exit(EXIT_FAILURE);
	}

	shadeProg.bindAttribLocation(0, "vertexPosition");
	shadeProg.bindAttribLocation(1, "vertexTextureCoordinate");
	shadeProg.bindAttribLocation(2, "vertexNormal");

	if (!shadeProg.link())
	{
		printf("Model shader program failed to link!\n%s\n", shadeProg.log().c_str());
		exit(EXIT_FAILURE);
	}

	printf("Printing model shader program information...\n");
	shadeProg.printActiveUniforms();
	shadeProg.printActiveAttribs();
	printf("\n");

	shadeProg.setUniform("lights[0].shadowMap", (GLint)0);
	shadeProg.setUniform("lights[1].shadowMap", (GLint)1);
	shadeProg.setUniform("lights[2].shadowMap", (GLint)2);
	shadeProg.setUniform("lights[3].shadowMap", (GLint)3);
	shadeProg.setUniform("lights[4].shadowMap", (GLint)4);
	shadeProg.setUniform("diffuseMap", (GLint)5);
}

void Model::loadShadowProg()
{
	if (shadowProg.isLinked())
	{
		return;
	}

	if (!shadowProg.compileShaderFromFile("Shaders/modelShadow.vert", GLSLShader::VERTEX))
	{
		printf("Model shadow vertex shader failed to compile!\n%s\n", shadowProg.log().c_str());
		exit(EXIT_FAILURE);
	}
	if (!shadowProg.compileShaderFromFile("Shaders/modelShadow.frag", GLSLShader::FRAGMENT))
	{
		printf("Model shadow fragment shader failed to compile!\n%s\n", shadowProg.log().c_str());
		exit(EXIT_FAILURE);
	}

	shadowProg.bindAttribLocation(0, "vertexPosition");

	if (!shadowProg.link())
	{
		printf("Model shadow shader program failed to link!\n%s\n", shadowProg.log().c_str());
		exit(EXIT_FAILURE);
	}

	printf("Printing model shadow shader program information...\n");
	shadowProg.printActiveUniforms();
	shadowProg.printActiveAttribs();
	printf("\n");
}

Model::~Model()
{
}

void Model::draw(Graphics const& graphics) const
{
	Camera::ptr camera = graphics.getCamera();

	mat4 modelViewMatrix = camera->getViewMatrix() * getModelMatrix();
	mat3 normalMatrix = inverseTranspose(mat3(modelViewMatrix));
	mat4 projectionMatrix = camera->getProjectionMatrix();

	shadeProg.setUniform("modelViewMatrix", modelViewMatrix);
	shadeProg.setUniform("normalMatrix", normalMatrix);
	shadeProg.setUniform("projectionMatrix", projectionMatrix);

	GLuint lightCount = 0;
	const static GLuint MAX_LIGHTS = 5;
	char uniformBuffer[128] = {0};
	BOOST_FOREACH(PointLight::ptr const& light, graphics.getPrimaryLights())
	{
		sprintf_s(uniformBuffer, "lights[%d].position", lightCount);
		shadeProg.setUniform(uniformBuffer, light->getPosition());

		sprintf_s(uniformBuffer, "lights[%d].intensity", lightCount);
		shadeProg.setUniform(uniformBuffer, light->getIntensity());

		sprintf_s(uniformBuffer, "shadowMatrices[%d]", lightCount);
		shadeProg.setUniform(uniformBuffer, light->getViewProjectionMatrix() * getModelMatrix());

		sprintf_s(uniformBuffer, "lights[%d].shadowMap", lightCount);
		shadeProg.setUniform(uniformBuffer, (GLint)lightCount);

		glActiveTexture(GL_TEXTURE0 + lightCount);
		glBindTexture(GL_TEXTURE_2D, light->getShadowTexture());

		lightCount++;
		if (lightCount >= MAX_LIGHTS)
		{
			break;
		}
	}

	shadeProg.setUniform("numLights", lightCount);

	modelData->draw(shadeProg);
}

void Model::drawShadow(PointLight::ptr const& light) const
{
	mat4 modelViewProjectionMatrix = light->getProjectionMatrix() * light->getViewMatrix() * getModelMatrix();

	shadowProg.setUniform("modelViewProjectionMatrix", modelViewProjectionMatrix);

	modelData->drawShadow();
}

void Model::setPosition(vec3 const& position)
{
	if (position != this->position)
	{
		this->position = position;
		validModelMatrix = false;
	}
}

void Model::setRotation(vec3 const& rotation)
{
	vec3 tRot = glm::mod(rotation, 360);
	if (tRot != this->rotation)
	{
		this->rotation = tRot;
		validModelMatrix = false;
	}
}

void Model::setScale(vec3 const& scale)
{
	if (scale != this->scale)
	{
		this->scale = scale;
		validModelMatrix = false;
	}
}

vec3 const& Model::getPosition() const
{
	return position;
}

vec3 const& Model::getRotation() const
{
	return rotation;
}

vec3 const& Model::getScale() const
{
	return scale;
}

mat4 const& Model::getModelMatrix() const
{
	if (!validModelMatrix)
	{
		mat4 scaleMatrix = glm::scale(mat4(), scale);

		mat4 rotateX = rotate(mat4(), rotation.x, vec3(1, 0, 0));
		mat4 rotateY = rotate(mat4(), rotation.y, vec3(0, 1, 0));
		mat4 rotateZ = rotate(mat4(), rotation.z, vec3(0, 0, 1));

		mat4 translateMatrix = translate(mat4(), position);

		modelMatrix = translateMatrix * rotateZ * rotateY * rotateX * scaleMatrix;

		validModelMatrix = true;
	}

	return modelMatrix;
}
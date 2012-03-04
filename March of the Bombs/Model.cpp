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
#include "Model3DS.h"

Model::Model(ModelData::ptr const& model)
	: modelData(model), scale(1.f), tint(0.f), selected(false),
	validModelMatrix(false), validInverseModelMatrix(false)
{
}

Model::~Model()
{
}

void Model::addInstance()
{
	modelData->addInstanceToDraw(getModelMatrix(), tint);
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
		validInverseModelMatrix = false;
	}

	return modelMatrix;
}

mat4 const& Model::getInverseModelMatrix() const
{
	if (!validInverseModelMatrix)
	{
		inverseModelMatrix = glm::inverse(getModelMatrix());

		validInverseModelMatrix = true;
	}

	return inverseModelMatrix;
}

void Model::setSelected(bool selected)
{
	this->selected = selected;
}

bool Model::isSelected() const
{
	return selected;
}

void Model::setTint(glm::vec4 const& tint)
{
	this->tint = tint;
}

glm::vec4 const& Model::getTint() const
{
	return tint;
}

bool Model::rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const
{
	mat4 invMat = getInverseModelMatrix();
	vec3 modelOrigin = vec3(invMat * glm::vec4(origin, 1));
	vec3 modelDirection = vec3(invMat * glm::vec4(direction, 0));

	return modelData->getBoundingBox().rayIntersect(modelOrigin, modelDirection, distance);
}
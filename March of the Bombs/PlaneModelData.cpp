#include "PlaneModelData.h"

using glm::vec2;
using glm::vec3;

#include <boost/foreach.hpp>

const unsigned short PlaneModelData::NUM_INDICES = 4;
PlaneModelData::ptr PlaneModelData::instance;

void PlaneModelData::createVBO()
{
	vec3 positionData[4] =
	{
		vec3(0, 0, 0),
		vec3(0, 0, 1),
		vec3(1, 0, 1),
		vec3(1, 0, 0)
	};

	vec3 normalData[4] =
	{
		vec3(0, 1, 0),
		vec3(0, 1, 0),
		vec3(0, 1, 0),
		vec3(0, 1, 0)
	};

	vec2 texData[4] =
	{
		vec2(0, 1),
		vec2(0, 0),
		vec2(1, 0),
		vec2(1, 1)
	};

	unsigned char indexData[NUM_INDICES] =
	{
		0, 1, 3, 2
	};

	GLuint vertexBuffers[4];
	glGenBuffers(4, vertexBuffers);

	vertexVBO = vertexBuffers[0];
	texCoordVBO = vertexBuffers[1];
	normalVBO = vertexBuffers[2];
	indexVBO = vertexBuffers[3];

	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalData), normalData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texData), texData, GL_STATIC_DRAW);


	glGenVertexArrays(1, &modelVAO);
	glBindVertexArray(modelVAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PlaneModelData::setMaterial()
{
	material.ambient[0] = 0;
	material.ambient[1] = 0;
	material.ambient[2] = 0;

	material.diffuse[0] = 0.9f;
	material.diffuse[1] = 0.9f;
	material.diffuse[2] = 0.9f;

	material.specular[0] = 0.1f;
	material.specular[1] = 0.1f;
	material.specular[2] = 0.1f;

	material.shininess = 20.f;

	material.texture1_map.user_ptr = (void*)texture->getHandle();
}

void PlaneModelData::useMaterial(GLSLProgram const& prog) const
{
	prog.setUniform("material.ambient", *reinterpret_cast<glm::vec3 const*>(material.ambient));
	prog.setUniform("material.diffuse", *reinterpret_cast<glm::vec3 const*>(material.diffuse));
	prog.setUniform("material.specular", *reinterpret_cast<glm::vec3 const*>(material.specular));
	prog.setUniform("material.shininess", material.shininess);

	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, (GLuint) material.texture1_map.user_ptr);
}

PlaneModelData::PlaneModelData()
	: vertexVBO(0), normalVBO(0), texCoordVBO(0), indexVBO(0), modelVAO(0),
	texture(GLTexture::getTexture(L"Images/GroundPlane.png")),
	boundingBox(glm::vec3(0.5f, 0.f, 0.5f), glm::vec3(0.5f, 0.f, 0.5f))
{
	createVBO();
	setMaterial();
}

PlaneModelData::~PlaneModelData()
{
	if (vertexVBO)
	{
		glDeleteBuffers(1, &vertexVBO);
	}

	if (normalVBO)
	{
		glDeleteBuffers(1, &normalVBO);
	}

	if (texCoordVBO)
	{
		glDeleteBuffers(1, &texCoordVBO);
	}

	if (indexVBO)
	{
		glDeleteBuffers(1, &texCoordVBO);
	}

	if (modelVAO)
	{
		glDeleteVertexArrays(1, &modelVAO);
	}
}

void PlaneModelData::addInstanceToDraw(glm::mat4 const& modelMatrix, bool selected)
{
	drawInst.push_back(DrawInstance(modelMatrix, selected));
}

void PlaneModelData::clearInstancesToDraw()
{
	drawInst.clear();
}

void PlaneModelData::drawInstances(GLSLProgram const& prog) const
{
	glBindVertexArray(modelVAO);

	useMaterial(prog);

	BOOST_FOREACH(DrawInstance const& inst, drawInst)
	{
		prog.setUniform("modelMatrix", inst.modelMatrix);
		prog.setUniform("selected", inst.selected);
		glDrawElements(GL_TRIANGLE_STRIP, NUM_INDICES, GL_UNSIGNED_BYTE, NULL);
	}

	glBindVertexArray(0);
}

void PlaneModelData::drawInstancesShadow(GLSLProgram const& prog) const
{
	glBindVertexArray(modelVAO);

	BOOST_FOREACH(DrawInstance const& inst, drawInst)
	{
		prog.setUniform("modelMatrix", inst.modelMatrix);
		glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_BYTE, NULL);
	}

	glBindVertexArray(0);
}

PlaneModelData::ptr PlaneModelData::getInstance()
{
	if (!instance)
	{
		instance = PlaneModelData::ptr(new PlaneModelData());
	}

	return instance;
}

BoundingBox PlaneModelData::getBoundingBox() const
{
	return boundingBox;
}
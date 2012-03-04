#include "Model3DS.h"

#include <glm/glm.hpp>
#include <vector>
using std::vector;

#include <list>
#include <map>

#include <boost/foreach.hpp>

Model3DS::MaterialGroup::MaterialGroup()
	: count(0), startIndex(0)
{
}

void Model3DS::MaterialGroup::use(GLSLProgram const& prog) const
{
	prog.setUniform("material.ambient", *reinterpret_cast<glm::vec3 const*>(material.ambient));
	prog.setUniform("material.diffuse", *reinterpret_cast<glm::vec3 const*>(material.diffuse));
	prog.setUniform("material.specular", *reinterpret_cast<glm::vec3 const*>(material.specular));
	prog.setUniform("material.shininess", 40.f);

	if (texture)
	{
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, texture->getHandle());
	}
}

void Model3DS::MaterialGroup::addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint)
{
	drawInst.push_back(DrawInstance(modelMatrix, tint));
}

void Model3DS::MaterialGroup::clearInstancesToDraw()
{
	drawInst.clear();
}

void Model3DS::MaterialGroup::drawInstances(GLSLProgram const& prog) const
{
	use(prog);

	BOOST_FOREACH(DrawInstance const& inst, drawInst)
	{
		prog.setUniform("modelMatrix", inst.modelMatrix);
		prog.setUniform("tint", inst.tint);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(startIndex * sizeof(unsigned short)));
	}
}

void Model3DS::MaterialGroup::drawInstancesShadow(GLSLProgram const& prog) const
{
	BOOST_FOREACH(DrawInstance const& inst, drawInst)
	{
		prog.setUniform("modelMatrix", inst.modelMatrix);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(startIndex * sizeof(unsigned short)));
	}
}

void Model3DS::fixBoundingBox(Lib3dsFile* modelFile)
{
	glm::vec3 pMin, pMax;
	lib3ds_file_bounding_box_of_objects(modelFile, true, false, false, &pMin[0], &pMax[0]);

	boundingBox.position = (pMin + pMax) * 0.5f;
	boundingBox.halfSize = (pMax - pMin) * 0.5f;
}

Model3DS::Model3DS(std::string const& fileName)
	: vertexVBO(0), normalVBO(0), texCoordVBO(0), indexVBO(0), modelVAO(0), boundingBox(glm::vec3(0.5f), glm::vec3(0.5f))
{
	Lib3dsFile* modelFile = lib3ds_file_open(fileName.c_str());

	if (!modelFile)
	{
		throw "Could not load model";
	}

	createVBO(modelFile);
	fixBoundingBox(modelFile);
	lib3ds_file_free(modelFile);
}

Model3DS::~Model3DS()
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

class compareVec3
{
public:
	bool operator()(glm::vec3 const& lhs, glm::vec3 const& rhs)
	{
		if (lhs.x < rhs.x) return true;
		if (lhs.x > rhs.x) return false;
		if (lhs.y < rhs.y) return true;
		if (lhs.y > rhs.y) return false;
		if (lhs.z < rhs.z) return true;
		return false;
	}
};

void Model3DS::createVBO(Lib3dsFile* modelFile)
{
	vector<glm::vec3> vertices;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> indexedNormals;
	vector<unsigned short> indices;
	
	for (int i = 0; i < modelFile->nmeshes; i++)
	{
		Lib3dsMesh* mesh = modelFile->meshes[i];

		unsigned int baseVertex = vertices.size();

		unsigned int numVert = mesh->nvertices;
		vertices.reserve(vertices.size() + numVert);
		texCoords.reserve(texCoords.size() + numVert);

		for (unsigned int j = 0; j < numVert; j++)
		{
			vertices.push_back(*reinterpret_cast<glm::vec3*>(mesh->vertices[j]));
			texCoords.push_back(*reinterpret_cast<glm::vec2*>(mesh->texcos[j]));
		}

		std::map<glm::vec3, std::list<unsigned short>, compareVec3> doubleVertices;
		for (unsigned int j = baseVertex; j < vertices.size(); j++)
		{
			doubleVertices[vertices[j]].push_back(j);
		}

		vector<glm::vec3> faceNormals(mesh->nfaces);
		lib3ds_mesh_calculate_face_normals(mesh, reinterpret_cast<float(*)[3]>(&faceNormals[0]));

		int fCount = 0;
		indexedNormals.resize(indexedNormals.size() + numVert);
		BOOST_FOREACH(glm::vec3 const& normal, faceNormals)
		{
			Lib3dsFace const& face = mesh->faces[fCount];

			for (unsigned int j = 0; j < 3; j++)
			{
				glm::vec3 pos = vertices[baseVertex + face.index[j]];

				BOOST_FOREACH(unsigned short normInd, doubleVertices[pos])
				{
					indexedNormals[normInd] += normal;
				}
			}

			fCount++;
		}

		for (unsigned int j = baseVertex; j < indexedNormals.size(); j++)
		{
			indexedNormals[j] = glm::normalize(indexedNormals[j]);
		}

		indices.reserve(indices.size() + mesh->nfaces * 3);
		int matIndex = -1;
		MaterialGroup* currentGroup = NULL;
		for (int i = 0; i < mesh->nfaces; i++)
		{
			Lib3dsFace& face = mesh->faces[i];

            if (matIndex != face.material)
			{
				matIndex = face.material;

				if (matIndex != -1)
				{
					groups.push_back(MaterialGroup());

					currentGroup = &groups.back();
					currentGroup->startIndex = indices.size();
					currentGroup->material = *modelFile->materials[matIndex];
				}
			}

			for (int j = 0; j < 3; j++)
			{
				indices.push_back(baseVertex + face.index[j]);
			}

			currentGroup->count += 3;
		}
	}

	GLuint vertexBuffers[4];
	glGenBuffers(4, vertexBuffers);

	vertexVBO = vertexBuffers[0];
	normalVBO = vertexBuffers[1];
	texCoordVBO = vertexBuffers[2];
	indexVBO = vertexBuffers[3];

	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(glm::vec3), indexedNormals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);


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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model3DS::addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint)
{
	BOOST_FOREACH(MaterialGroup& group, groups)
	{
		group.addInstanceToDraw(modelMatrix, tint);
	}
}

void Model3DS::clearInstancesToDraw()
{
	BOOST_FOREACH(MaterialGroup& group, groups)
	{
		group.clearInstancesToDraw();
	}
}

void Model3DS::drawInstances(GLSLProgram const& prog) const
{
	glBindVertexArray(modelVAO);

	BOOST_FOREACH(MaterialGroup const& material, groups)
	{
		material.drawInstances(prog);
	}

	glBindVertexArray(0);
}

void Model3DS::drawInstancesShadow(GLSLProgram const& prog) const
{
	glBindVertexArray(modelVAO);

	BOOST_FOREACH(MaterialGroup const& material, groups)
	{
		material.drawInstancesShadow(prog);
	}

	glBindVertexArray(0);
}

BoundingBox Model3DS::getBoundingBox() const
{
	return boundingBox;
}
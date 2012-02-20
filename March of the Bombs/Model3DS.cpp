#include "Model3DS.h"

#include <glm/glm.hpp>
#include <vector>
using std::vector;

#include <boost/foreach.hpp>

Model3DS::MaterialGroup::MaterialGroup()
	: count(0), startIndex(0), material(NULL)
{
}

void Model3DS::MaterialGroup::use(GLSLProgram const& prog) const
{
	prog.setUniform("ambient", *reinterpret_cast<glm::vec3*>(material->ambient));
	prog.setUniform("diffuse", *reinterpret_cast<glm::vec3*>(material->diffuse));
	prog.setUniform("specular", *reinterpret_cast<glm::vec3*>(material->specular));
}

Model3DS::Model3DS(std::string const& fileName)
	: vertexVBO(0), normalVBO(0), texCoordVBO(0), indexVBO(0), modelVAO(0)
{
	model = lib3ds_file_open(fileName.c_str());

	if (!model)
	{
		throw "Could not load model";
	}
}

Model3DS::~Model3DS()
{
	lib3ds_file_free(model);

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

void Model3DS::draw(GLSLProgram const& prog) const
{
	glBindVertexArray(modelVAO);

	BOOST_FOREACH(MaterialGroup const& material, groups)
	{
		material.use(prog);
		glDrawElements(GL_TRIANGLES, material.count, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(material.startIndex * sizeof(unsigned short)));
	}

	glBindVertexArray(0);
}

void Model3DS::drawShadow() const
{
	glBindVertexArray(modelVAO);

	BOOST_FOREACH(MaterialGroup const& material, groups)
	{
		glDrawElements(GL_TRIANGLES, material.count, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(material.startIndex * sizeof(unsigned short)));
	}

	glBindVertexArray(0);
}

void Model3DS::createVBO()
{
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> texCoords;
	vector<unsigned short> indices;
	
	for (int i = 0; i < model->nmeshes; i++)
	{
		Lib3dsMesh* mesh = model->meshes[i];

		unsigned int baseVertex = vertices.size();

		unsigned int numVert = mesh->nvertices;
		vertices.reserve(vertices.size() + numVert);
		texCoords.reserve(texCoords.size() + numVert);

		unsigned int normalPos = normals.size();
		normals.resize(normals.size() + numVert);
		lib3ds_mesh_calculate_vertex_normals(mesh, reinterpret_cast<float(*)[3]>(&normals[normalPos]));

		for (unsigned int j = 0; j < numVert; j++)
		{
			vertices.push_back(*reinterpret_cast<glm::vec3*>(mesh->vertices[j]));
			texCoords.push_back(*reinterpret_cast<glm::vec2*>(mesh->texcos[j]));
		}

		indices.reserve(indices.size() + numVert);
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

					currentGroup = &groups.front();
					currentGroup->startIndex = indices.size();
					currentGroup->material = model->materials[matIndex];
				}
			}

			for (int j = 0; j < 3; j++)
			{
				indices.push_back(face.index[j]);
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
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);


	glGenVertexArrays(1, &modelVAO);
	glBindVertexArray(modelVAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
#pragma once

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include <lib3ds/lib3ds.h>
#pragma comment(lib, "lib3ds")

#include <GL/glew.h>

#include "ModelData.h"
#include "GLSLProgram.h"
#include "GLTexture.h"

class Model3DS
	: public ModelData
{
public:
	typedef boost::shared_ptr<Model3DS> ptr;

protected:
	class MaterialGroup
	{
	private:
		class DrawInstance
		{
		public:
			glm::mat4 modelMatrix;

			DrawInstance(glm::mat4 const& modelMatrix)
				: modelMatrix(modelMatrix)
			{
			}
		};
		std::vector<DrawInstance> drawInst;

	public:
		unsigned int count;
		unsigned int startIndex;
		Lib3dsMaterial material;
		GLTexture::ptr texture;

		MaterialGroup();

		void use(GLSLProgram const& prog) const;

		virtual void addInstanceToDraw(glm::mat4 const& modelMatrix);
		virtual void clearInstancesToDraw();

		virtual void drawInstances(GLSLProgram const& prog) const;
		virtual void drawInstancesShadow(GLSLProgram const& prog) const;
	};
	std::vector<MaterialGroup> groups;

	Model3DS(std::string const& fileName);

private:
	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint texCoordVBO;
	GLuint indexVBO;
	GLuint modelVAO;

	virtual void createVBO(Lib3dsFile* modelFile);

public:
	virtual ~Model3DS();

	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix);
	virtual void clearInstancesToDraw();

	virtual void drawInstances(GLSLProgram const& prog) const;
	virtual void drawInstancesShadow(GLSLProgram const& prog) const;
};
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
	public:
		unsigned int count;
		unsigned int startIndex;
		Lib3dsMaterial material;
		GLTexture::ptr texture;

		MaterialGroup();

		void use(GLSLProgram const& prog) const;
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

	virtual void draw(GLSLProgram const& prog) const;
	virtual void drawShadow() const;
};
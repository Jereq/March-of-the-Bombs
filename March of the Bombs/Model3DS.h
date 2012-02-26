#pragma once

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <lib3ds/lib3ds.h>
#pragma comment(lib, "lib3ds")

#include <GL/glew.h>

#include "GLSLProgram.h"

class Model3DS
{
protected:
	class MaterialGroup
	{
	public:
		unsigned int count;
		unsigned int startIndex;
		Lib3dsMaterial material;

		MaterialGroup();

		void use(GLSLProgram const& prog) const;
	};

public:
	std::vector<MaterialGroup> groups;

	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint texCoordVBO;
	GLuint indexVBO;
	GLuint modelVAO;

	virtual void createVBO(Lib3dsFile* modelFile);

public:
	typedef boost::shared_ptr<Model3DS> ptr;

	Model3DS(std::string const& fileName);
	virtual ~Model3DS();

	virtual void draw(GLSLProgram const& prog) const;
	virtual void drawShadow() const;
};
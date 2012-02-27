#pragma once

#include "Modeldata.h"

#include <lib3ds/lib3ds.h>

class BlockModelData :
	public ModelData
{
public:
	typedef boost::shared_ptr<BlockModelData> ptr;

private:
	const static unsigned short NUM_INDICES;

	static ptr instance;

	Lib3dsMaterial material;

	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint texCoordVBO;
	GLuint indexVBO;
	GLuint modelVAO;

	virtual void createVBO();

protected:
	BlockModelData();

public:
	virtual ~BlockModelData();
	
	virtual void draw(GLSLProgram const& prog) const;
	virtual void drawShadow() const;

	static ptr getInstance();
};
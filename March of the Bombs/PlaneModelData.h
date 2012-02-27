#pragma once

#include "ModelData.h"

#include <lib3ds/lib3ds.h>

#include "GLTexture.h"

class PlaneModelData :
	public ModelData
{
public:
	typedef boost::shared_ptr<PlaneModelData> ptr;

private:
	const static unsigned short NUM_INDICES;

	static ptr instance;

	Lib3dsMaterial material;
	GLTexture::ptr texture;

	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint texCoordVBO;
	GLuint indexVBO;
	GLuint modelVAO;

	virtual void createVBO();
	virtual void setMaterial();
	virtual void useMaterial(GLSLProgram const& prog) const;

protected:
	PlaneModelData();

public:
	virtual ~PlaneModelData();
	
	virtual void draw(GLSLProgram const& prog) const;
	virtual void drawShadow() const;

	static ptr getInstance();
};
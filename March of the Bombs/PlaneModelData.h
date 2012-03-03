#pragma once

#include "ModelData.h"

#include <list>
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

	class DrawInstance
	{
	public:
		const glm::mat4 modelMatrix;

		DrawInstance(glm::mat4 const& modelMatrix)
			: modelMatrix(modelMatrix)
		{
		}
	};
	std::list<DrawInstance> drawInst;

	Lib3dsMaterial material;
	GLTexture::ptr texture;

	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint texCoordVBO;
	GLuint indexVBO;
	GLuint modelVAO;

	BoundingBox boundingBox;

	virtual void createVBO();
	virtual void setMaterial();
	virtual void useMaterial(GLSLProgram const& prog) const;

protected:
	PlaneModelData();

public:
	virtual ~PlaneModelData();

	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix);
	virtual void clearInstancesToDraw();

	virtual void drawInstances(GLSLProgram const& prog) const;
	virtual void drawInstancesShadow(GLSLProgram const& prog) const;

	static ptr getInstance();

	BoundingBox getBoundingBox() const;
};
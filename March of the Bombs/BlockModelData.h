#pragma once

#include "ModelData.h"

#include <vector>
#include <lib3ds/lib3ds.h>

#include "GLTexture.h"

class BlockModelData :
	public ModelData
{
public:
	typedef boost::shared_ptr<BlockModelData> ptr;

private:
	const static unsigned short NUM_INDICES;

	static ptr hardInstance;
	static ptr softInstance;

	class DrawInstance
	{
	public:
		glm::mat4 modelMatrix;
		glm::vec4 tint;

		DrawInstance(glm::mat4 const& modelMatrix, glm::vec4 const& tint)
			: modelMatrix(modelMatrix), tint(tint)
		{
		}
	};
	std::vector<DrawInstance> drawInst;

	Lib3dsMaterial material;
	GLTexture::ptr texture;

	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint texCoordVBO;
	GLuint indexVBO;
	GLuint modelVAO;

	BoundingBox boundingBox;

	virtual void createVBO();
	virtual void useMaterial(GLSLProgram const& prog) const;

protected:
	BlockModelData(GLTexture::ptr const& texture, Lib3dsMaterial const& material);

public:
	virtual ~BlockModelData();

	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint);
	virtual void clearInstancesToDraw();

	virtual void drawInstances(GLSLProgram const& prog) const;
	virtual void drawInstancesShadow(GLSLProgram const& prog) const;

	static ptr getHardInstance();
	static ptr getSoftInstance();

	BoundingBox getBoundingBox() const;
};
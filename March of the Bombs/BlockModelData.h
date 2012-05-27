#pragma once

#include "ModelData.h"

#include <vector>
#include <lib3ds/lib3ds.h>

#include "GLTexture.h"

/**
 * BlockModelData contains the raw data needed to draw a block.
 */
class BlockModelData :
	public ModelData
{
public:
	typedef boost::shared_ptr<BlockModelData> ptr;

private:
	/// Size of index array
	const static unsigned short NUM_INDICES;

	/// The static instance for drawing hard blocks
	static ptr hardInstance;
	/// The static instance for drawing soft blocks
	static ptr softInstance;

	/**
	 * Represents a single instance of a block model to draw.
	 */
	class DrawInstance
	{
	public:
		glm::mat4 modelMatrix;
		glm::vec4 tint;

		/// Constructor.
		DrawInstance(glm::mat4 const& modelMatrix, glm::vec4 const& tint)
			: modelMatrix(modelMatrix), tint(tint)
		{
		}
	};
	/// Collection of instances to draw using this block data
	std::vector<DrawInstance> drawInst;

	/// The material properties of this model data
	Lib3dsMaterial material;
	/// The texture this model uses. Must be held onto, or the memory will be released.
	GLTexture::ptr texture;

	/// Vertex position buffer handle
	GLuint vertexVBO;
	/// Vertex normal buffer handle
	GLuint normalVBO;
	/// Texture coordinates buffer handle
	GLuint texCoordVBO;
	/// Index buffer handle
	GLuint indexVBO;
	/// Vertex array object handle
	GLuint modelVAO;

	/// Bounding box that contains the model
	BoundingBox boundingBox;

	/// Set up and fill buffers with data
	virtual void createVBO();
	/// Use the models material with the given shader program
	virtual void useMaterial(GLSLProgram const& prog) const;

protected:
	/// Constructor. Sets up all buffers, the material and the bounding box
	BlockModelData(GLTexture::ptr const& texture, Lib3dsMaterial const& material);

public:
	/// Destructor.
	virtual ~BlockModelData();

	/// Adds the given instance to the list of instances to draw
	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint);
	/// Clears the list of instances to draw
	virtual void clearInstancesToDraw();

	/// Draw all instances normally
	virtual void drawInstances(GLSLProgram const& prog, Frustum const& cullFrustum) const;
	/// Draw all instances from a lights perspective onto a shadow map
	virtual void drawInstancesShadow(GLSLProgram const& prog, Frustum const& cullFrustum) const;

	/// Get a singleton instance of a hard block
	static ptr getHardInstance();
	/// Get a singleton instance of a soft block
	static ptr getSoftInstance();

	/// Get a base bounding box for the model
	BoundingBox getBoundingBox() const;
};
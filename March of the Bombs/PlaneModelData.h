#pragma once

#include "ModelData.h"

#include <list>
#include <lib3ds/lib3ds.h>

#include "GLTexture.h"

/**
 * PlaneModelData represents the raw data for a plane model.
 */
class PlaneModelData :
	public ModelData
{
public:
	typedef boost::shared_ptr<PlaneModelData> ptr;

private:
	const static unsigned short NUM_INDICES;

	/// Static instance shared between all planes
	static ptr instance;

	/**
	 * A instance of the model data to be drawn.
	 */
	class DrawInstance
	{
	public:
		const glm::mat4 modelMatrix;
		glm::vec4 tint;

		/// Constructor.
		DrawInstance(glm::mat4 const& modelMatrix, glm::vec4 const& tint)
			: modelMatrix(modelMatrix), tint(tint)
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

	/// Set up and fill the buffers with data
	virtual void createVBO();
	/// Set up the material
	virtual void setMaterial();
	/// Use the material with the given shader program
	virtual void useMaterial(GLSLProgram const& prog) const;

protected:
	/// Constructor.
	PlaneModelData();

public:
	/// Destructor.
	virtual ~PlaneModelData();

	/// Add an instance to the list to draw
	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint);
	/// Clear the list of instances to draw
	virtual void clearInstancesToDraw();

	/// Draw instances normally
	virtual void drawInstances(GLSLProgram const& prog) const;
	/// Draw instances from the lights perspective onto a shadow map
	virtual void drawInstancesShadow(GLSLProgram const& prog) const;

	/// Get a singleton instance
	static ptr getInstance();

	/// Get the base bounding box for the plane
	BoundingBox getBoundingBox() const;
};
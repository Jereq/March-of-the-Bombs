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

/**
 * Model3DS represents some of the data from a .3ds model file.
 */
class Model3DS
	: public ModelData
{
public:
	typedef boost::shared_ptr<Model3DS> ptr;

protected:
	/**
	 * MaterialGroup represents a part of the model that uses the same material.
	 */
	class MaterialGroup
	{
	private:
		/**
		 * DrawInstant is a single instance of the model to draw, with a transformation and a tint.
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
		std::vector<DrawInstance> drawInst;

	public:
		unsigned int count;
		unsigned int startIndex;
		Lib3dsMaterial material;
		GLTexture::ptr texture;

		/// Constructor.
		MaterialGroup();

		/// Uses the material together with the shader program
		void use(GLSLProgram const& prog) const;

		/// Add a´n instance to the list of instances to draw
		virtual void addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint);
		/// Clear the list of instances to draw
		virtual void clearInstancesToDraw();

		/// Draw the list of instances normally
		virtual void drawInstances(GLSLProgram const& prog, Frustum const& cullFrustum, BoundingBox const& boundingBox) const;
		/// Draw the list of instances from the lights perspective onto a shadow map
		virtual void drawInstancesShadow(GLSLProgram const& prog, Frustum const& cullFrustum, BoundingBox const& boundingBox) const;
	};
	std::vector<MaterialGroup> groups;

	/**
	 * Constructor. Attempt to load the file. Throws a string if the model file cound not be loaded.
	 */
	Model3DS(std::string const& fileName);

private:
	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint texCoordVBO;
	GLuint indexVBO;
	GLuint modelVAO;

	BoundingBox boundingBox;

	/// Create the vertex buffers and fill them with data from the model file
	virtual void createVBO(Lib3dsFile* modelFile);
	
	/// Adjust the boundingbox to fit around the model in the modelfile
	void fixBoundingBox(Lib3dsFile* modelFile);

public:
	/// Destructor.
	virtual ~Model3DS();

	/// Add an instance to the list of instances to draw
	virtual void addInstanceToDraw(glm::mat4 const& modelMatrix, glm::vec4 const& tint);
	/// Clear the list of instances to draw
	virtual void clearInstancesToDraw();

	/// Draw the instances normally
	virtual void drawInstances(GLSLProgram const& prog, Frustum const& cullFrustum) const;
	/// Draw the instances from the lights perspective onto a shadow map
	virtual void drawInstancesShadow(GLSLProgram const& prog, Frustum const& cullFrustum) const;

	/// Get the base bounding box for the model
	virtual BoundingBox getBoundingBox() const;
};
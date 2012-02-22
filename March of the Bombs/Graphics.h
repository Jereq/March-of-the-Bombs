#pragma once

#include <boost/shared_ptr.hpp>

#include <vector>
#include <list>

#include "GLTexture.h"
#include "GLSLProgram.h"
#include "Rectanglef.h"
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"

class Graphics
{
private:
	struct TextureInstance
	{
		const GLTexture::ptr texture;
		const Rectanglef target;
		const float depth;

		TextureInstance(GLTexture::ptr const& texture, Rectanglef const& target, float depth)
			: texture(texture), target(target), depth(depth)
		{
		}
	};

	GLSLProgram prog2D;
	Camera::ptr camera;
	std::vector<PointLight::ptr> primaryLights;

	std::list<Model::ptr> models;
	std::list<TextureInstance> textureInstances;

	GLuint textureBuffers2D[2];
	GLuint texture2DVAO;

	void drawTextureInstance(TextureInstance const& texInst) const;
	void loadShaders();
	void prepareTextureBuffers();

public:
	typedef boost::shared_ptr<Graphics> ptr;

	Graphics();
	~Graphics();

	void drawTexture(GLTexture::ptr const& texture, Rectanglef const& target, float depth);
	void drawModel(Model::ptr const& model);
	void render();

	Camera::ptr getCamera() const;

	void addPrimaryLight(PointLight::ptr const& light);
	std::vector<PointLight::ptr> const& getPrimaryLights() const;

	void updateViewport();
};
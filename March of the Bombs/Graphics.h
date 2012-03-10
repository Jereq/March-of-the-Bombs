#pragma once

#include <boost/shared_ptr.hpp>

#include <vector>
#include <list>
#include <set>

#include "TextureSection.h"
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
		const TextureSection texture;
		const Rectanglef target;
		const float depth;

		TextureInstance(TextureSection const& texture, Rectanglef const& target, float depth)
			: texture(texture), target(target), depth(depth)
		{
		}

		bool operator<(TextureInstance const& rhs) const
		{
			if (depth > rhs.depth) return true;
			if (depth < rhs.depth) return false;
			if (texture < rhs.texture) return true;
			if (rhs.texture < texture) return false;
			if (target < rhs.target) return true;
			return false;
		}
	};

	GLSLProgram prog2D;
	GLSLProgram progModelShade;
	GLSLProgram progModelShadow;

	Camera::ptr camera;
	PointLight::ptr light;

	std::set<ModelData::ptr> modelDatas;
	std::set<TextureInstance> textureInstances;
	TextureSection::ptr backgroundTexture;

	GLuint textureBuffers2D[2];
	GLuint texture2DVAO;

	void drawTextureInstance(TextureInstance const& texInst) const;

	void loadShaders();
	void load2DShaders();
	void loadModelShadeShaders();
	void loadModelShadowShaders();

	void prepareTextureBuffers();

	void setupModelShader();
	void setupModelShadowShader(PointLight::ptr const& light);

public:
	typedef boost::shared_ptr<Graphics> ptr;

	Graphics();
	~Graphics();

	void drawTexture(TextureSection const& texture, Rectanglef const& target, float depth);
	void drawModel(Model::ptr const& model);
	void render();

	Camera::ptr getCamera() const;

	PointLight::ptr const& getLight() const;
	void setLight(PointLight::ptr const& light);

	TextureSection::ptr const& getBackground() const;
	void setBackground(TextureSection::ptr const& background);

	void updateViewport();
};
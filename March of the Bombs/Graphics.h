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
		const glm::vec3 color;
		const Rectanglef target;
		const float depth;

		TextureInstance(TextureSection const& texture, glm::vec3 const& color, Rectanglef const& target, float depth)
			: texture(texture), color(color), target(target), depth(depth)
		{
		}

		bool operator<(TextureInstance const& rhs) const
		{
			if (depth > rhs.depth) return true;
			if (depth < rhs.depth) return false;
			if (texture < rhs.texture) return true;
			if (rhs.texture < texture) return false;
			if (color.x < rhs.color.x) return true;
			if (color.x > rhs.color.x) return false;
			if (color.y < rhs.color.y) return true;
			if (color.y > rhs.color.y) return false;
			if (color.z < rhs.color.z) return true;
			if (color.z > rhs.color.z) return false;
			if (target < rhs.target) return true;
			return false;
		}
	};

	struct BillboardInstance
	{
		const TextureSection texture;
		const glm::vec3 position;
		const glm::vec2 size;

		BillboardInstance(TextureSection const& texture, glm::vec3 const& position, glm::vec2 size)
			: texture(texture), position(position), size(size)
		{
		}

		bool operator<(BillboardInstance const& rhs) const
		{
			if (texture < rhs.texture) return true;
			if (rhs.texture < texture) return false;
			if (position.x < rhs.position.x) return true;
			if (position.x > rhs.position.x) return false;
			if (position.y < rhs.position.y) return true;
			if (position.y > rhs.position.y) return false;
			if (position.z < rhs.position.z) return true;
			if (position.z > rhs.position.z) return false;
			if (size.x < rhs.size.x) return true;
			if (size.x > rhs.size.x) return false;
			if (size.y < rhs.size.y) return true;
			return false;
		}
	};

	GLSLProgram prog2D;
	GLSLProgram progBillboard;
	GLSLProgram progModelShade;
	GLSLProgram progModelShadow;

	Camera::ptr camera;
	PointLight::ptr light;

	std::set<ModelData::ptr> modelDatas;
	std::set<TextureInstance> textureInstances;
	std::set<BillboardInstance> billboardInstances;
	TextureSection::ptr backgroundTexture;

	GLuint textureBuffers2D[2];
	GLuint texture2DVAO;

	const static size_t MAX_BILLBOARD_BUFFER_SIZE = 64;
	GLuint billboardBuffers[4];
	GLuint billboardVAO;

	void drawTextureInstance(TextureInstance const& texInst) const;
	void drawBillboardBatch(std::vector<BillboardInstance const*> const& batch) const;
	void drawBillboardInstances();

	void loadShaders();
	void load2DShaders();
	void loadBillboardShaders();
	void loadModelShadeShaders();
	void loadModelShadowShaders();

	void prepareTextureBuffers();

	void setupModelShader();
	void setupModelShadowShader(PointLight::ptr const& light);

public:
	typedef boost::shared_ptr<Graphics> ptr;

	Graphics();
	~Graphics();

	void drawTexture(TextureSection const& texture, Rectanglef const& target, float depth, glm::vec3 const& color);
	void drawBillboard(TextureSection const& texture, glm::vec3 const& position, glm::vec2 const& size);
	void drawModel(Model::ptr const& model);
	void render();

	Camera::ptr getCamera() const;

	PointLight::ptr const& getLight() const;
	void setLight(PointLight::ptr const& light);

	TextureSection::ptr const& getBackground() const;
	void setBackground(TextureSection::ptr const& background);

	void updateViewport();
};
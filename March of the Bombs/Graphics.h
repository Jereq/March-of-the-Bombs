#pragma once

#include <boost/shared_ptr.hpp>

#include <vector>

#include "GLTexture.h"
#include "GLSLProgram.h"
#include "Rectanglef.h"
#include "Camera.h"
#include "PointLight.h"

class Graphics
{
private:
	GLSLProgram prog2D;
	Camera::ptr camera;
	std::vector<PointLight::ptr> primaryLights;

	void loadShaders();

public:
	typedef boost::shared_ptr<Graphics> ptr;

	Graphics();

	void drawTexture(GLTexture::ptr texture, Rectanglef const& target, float depth);

	Camera::ptr getCamera() const;

	void addPrimaryLight(PointLight::ptr const& light);
	std::vector<PointLight::ptr> const& getPrimaryLights() const;
};
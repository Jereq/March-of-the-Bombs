#pragma once

#include "GLTexture.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

class Button
{
private:
	GLTexture::ptr unpressedTexture;		//storing buttons unpressedtexture under a name
	GLTexture::ptr pressedTexture;			//storing buttons pressedtexture under a name
	glm::vec4 color;						//using vec4 for more options later on, this as color
	Rectanglef posSizeRectangle;				//position- and size- rectangle 
	bool pressed;
	float depth;

public:
	Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectanglef rectangle, float depth);
	Button(GLTexture::ptr texture1, GLTexture::ptr texture2, Rectanglef rectangle, float depth, glm::vec4 color);
	virtual ~Button();
	void render(Graphics::ptr dT);
	void changeState();

	bool intersects(glm::vec2 const& point) const;
	bool isPressed() const;
};
#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

enum ButtonState
{
	Unused,
	Hovered,
	Used
};

class Button
{
private:
	TextureSection unpressedTexture;		//storing buttons unpressedtexture under a name
	TextureSection pressedTexture;			//storing buttons pressedtexture under a name
	glm::vec4 color;						//using vec4 for more options later on, this as color
	Rectanglef posSizeRectangle;				//position- and size- rectangle 
	ButtonState buttonState;
	float depth;

public:
	Button(TextureSection texture1, TextureSection texture2, Rectanglef rectangle, float depth);
	Button(TextureSection texture1, TextureSection texture2, Rectanglef rectangle, float depth, glm::vec4 color);
	virtual ~Button();
	void render(Graphics::ptr dT);
	ButtonState getState();
	void setState(ButtonState buttonState);

	bool intersects(glm::vec2 const& point) const;
	bool isPressed() const;
};
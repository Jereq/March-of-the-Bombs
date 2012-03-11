#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

#include "KeyboardEvent.h"

class TextField
{
private:
	TextureSection Background;
	Rectanglef PosRect;
	float depth;
	std::string Text;

	typedef std::map<char,TextureSection> image_map;
	static image_map keyMap;

public:
	
	TextField(TextureSection Background, Rectanglef PosRect, float depth);
	~TextField();

	void draw(Graphics::ptr graphics);
	void static keyMapping();
	void updateString(KeyboardEvent* keyEvent);
};


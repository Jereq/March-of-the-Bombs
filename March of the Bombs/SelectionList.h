#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

class SelectionList
{
private:
	TextureSection Background;
	Rectanglef PosRect;
	float depth;

public:
	typedef boost::shared_ptr<SelectionList> ptr;

	std::vector<std::string> GameList;
	SelectionList(TextureSection Background, Rectanglef PosRect, float depth);
	~SelectionList();
	void render(Graphics::ptr graphics);
	void updateString();

};


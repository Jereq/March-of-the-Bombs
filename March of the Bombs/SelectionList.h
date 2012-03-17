#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "FontandKeyMapping.h"
#include "Graphics.h"
#include "MouseButtonEvent.h"
#include <glm\glm.hpp>

class SelectionList
{
private:
	TextureSection Background;
	Rectanglef PosRect, StringHightRect;
	float depth;
	int index;
	FontandKeyMapping FaKM;

public:
	typedef boost::shared_ptr<SelectionList> ptr;

	std::vector<std::string> GameList;

	SelectionList(TextureSection Background, Rectanglef PosRect, float depth);
	~SelectionList();
	void render(Graphics::ptr graphics);
	void updateselection(MouseButtonEvent const* mbEvent);
	int getindex();
};


#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

#include "KeyboardEvent.h"


class SelectionList
{
private:


public:
	typedef boost::shared_ptr<SelectionList> ptr;

	SelectionList();
	~SelectionList();
	void render(Graphics::ptr graphics);
};


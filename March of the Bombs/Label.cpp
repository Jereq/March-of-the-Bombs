#include "Label.h"

#include <boost/foreach.hpp>

boost::shared_ptr<FontandKeyMapping> Label::font;

Label::Label(glm::vec2 const& position, glm::vec2 const& letterSize, std::string const& text,
	float depth, glm::vec3 const& color)
	: position(position), letterSize(letterSize), text(text), depth(depth), color(color)
{
	if (!font)
	{
		font.reset(new FontandKeyMapping());
	}
}

std::string const& Label::getText() const
{
	return text;
}

void Label::setText(std::string const& text)
{
	this->text = text;
}

glm::vec2 const& Label::getPosition() const
{
	return position;
}

void Label::setPosition(glm::vec2 const& position)
{
	this->position = position;
}

glm::vec2 const& Label::getLetterSize() const
{
	return letterSize;
}

void Label::setLetterSize(glm::vec2 const& letterSize)
{
	this->letterSize = letterSize;
}

float Label::getDepth() const
{
	return depth;
}

void Label::setDepth(float newDepth)
{
	depth = newDepth;
}

glm::vec3 const& Label::getColor() const
{
	return color;
}

void Label::setColor(glm::vec3 const& newColor)
{
	color = newColor;
}

void Label::draw(Graphics::ptr const& graphics)
{
	Rectanglef letterRect(glm::vec2(position.x, position.y - letterSize.y), letterSize);
	FontandKeyMapping::image_map& keyMap(font->getKeyMap());

	BOOST_FOREACH(char const& c, text)
	{
		if (c == '\n')
		{
			letterRect.setPosition(glm::vec2(position.x, letterRect.getPosition().y + letterSize.y));
		}
		else
		{
			if (keyMap.count(c) == 1)
			{
				graphics->drawTexture(keyMap[c], letterRect, depth, color);
			}

			letterRect.setPosition(letterRect.getPosition() + glm::vec2(letterSize.x, 0));
		}
	}
}
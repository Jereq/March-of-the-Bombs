#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Graphics.h"
#include "FontandKeyMapping.h"

class Label
{
private:
	static boost::shared_ptr<FontandKeyMapping> font;

	std::string text;
	glm::vec2 position;
	glm::vec2 letterSize;
	float depth;

public:
	Label(glm::vec2 const& position, glm::vec2 const& letterSize, std::string const& text, float depth);

	std::string const& getText() const;
	void setText(std::string const& text);

	glm::vec2 const& getPosition() const;
	void setPosition(glm::vec2 const& position);

	glm::vec2 const& getLetterSize() const;
	void setLetterSize(glm::vec2 const& size);

	float getDepth() const;
	void setDepth(float newDepth);

	void draw(Graphics::ptr const& graphics);
};
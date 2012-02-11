#pragma once

#include <glm/glm.hpp>

class Rectangle
{
private:
	glm::vec2 position;
	glm::vec2 size;

public:
	Rectangle();
	Rectangle(glm::vec2 const& position, glm::vec2 const& size);

	glm::vec2 getPosition() const;
	glm::vec2 getSize() const;

	void setPosition(glm::vec2 const& newPos);
	void setSize(glm::vec2 const& newSize);
};
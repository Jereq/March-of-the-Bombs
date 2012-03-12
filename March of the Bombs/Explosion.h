#pragma once

#include "Animation.h"
#include "Graphics.h"

class Explosion
{
private:
	Animation animation;
	glm::vec3 position;
	glm::vec2 size;

public:
	Explosion();
	Explosion(glm::vec3 const& position, glm::vec2 const& size, float duration);

	void update(float deltaTime);
	void draw(Graphics::ptr& graphics);

	bool isDone() const;
};
#pragma once

#include "Animation.h"
#include "Graphics.h"
#include "MChannel.h"

class Explosion
{
private:
	Animation animation;
	glm::vec3 position;
	glm::vec2 size;
	MChannel* sound;

public:
	Explosion();
	Explosion(glm::vec3 const& position, glm::vec2 const& size, float duration, MChannel* sound);

	void update(float deltaTime);
	void draw(Graphics::ptr& graphics);

	bool isDone() const;
};
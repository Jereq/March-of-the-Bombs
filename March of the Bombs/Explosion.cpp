#include "Explosion.h"

Explosion::Explosion()
{
}

Explosion::Explosion(glm::vec3 const& position, glm::vec2 const& size, float duration)
	: animation(TextureSection(L"Images/explosion.png"), glm::ivec2(8, 1), duration, false),
	position(position), size(size)
{
}

void Explosion::update(float deltaTime)
{
	animation.update(deltaTime);
}

void Explosion::draw(Graphics::ptr& graphics)
{
	graphics->drawBillboard(animation.getFrame(), position, size);
}

bool Explosion::isDone() const
{
	return animation.isDone();
}
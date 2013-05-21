#include "Explosion.h"

Explosion::Explosion()
{
}

Explosion::Explosion(glm::vec3 const& position, glm::vec2 const& size, float duration, MChannel* sound)
	: animation(TextureSection(L"Images/explosion.png"), glm::ivec2(8, 1), duration, false)
	, position(position)
	, size(size)
	, sound(sound)
{
}

void Explosion::update(float deltaTime)
{
	animation.update(deltaTime);

	if (sound && sound->getIsDone())
	{
		delete sound;
		sound = nullptr;
	}
}

void Explosion::draw(Graphics::ptr& graphics)
{
	graphics->drawBillboard(animation.getFrame(), position, size);
}

bool Explosion::isDone() const
{
	return animation.isDone() && (sound == nullptr || sound->getIsDone());
}
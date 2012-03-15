#include "Bomb.h"

#include "StandardBombModelData.h"
#include "Map.h"

Bomb::Bomb()
{
}

Bomb::Bomb(unsigned short owner, glm::vec3 const& ownerColor)
	: owner(owner), ownerColor(ownerColor),
	newHeading(false), selected(false), model(new Model(StandardBombModelData::getInstance()))
{
	model->setScale(glm::vec3(0.3f));
	model->setTint(glm::vec4(ownerColor, 0.3f));
}

unsigned short Bomb::getOwner() const
{
	return owner;
}

glm::vec3 const& Bomb::getPosition() const
{
	return model->getPosition();
}

glm::vec3 const& Bomb::getRotation() const
{
	return model->getRotation();
}

glm::vec3 const& Bomb::getVelocity() const
{
	return velocity;
}

void Bomb::setPosition(glm::vec3 const& pos)
{
	model->setPosition(pos);
}

void Bomb::setRotation(glm::vec3 const& rot)
{
	model->setRotation(rot);
}

void Bomb::setVelocity(glm::vec3 const& vel)
{
	velocity = vel;
}

bool Bomb::isSelected() const
{
	return selected;
}

void Bomb::setSelected(bool select)
{
	if (select)
	{
		selected = true;
		model->setTint(glm::vec4(0.5f, 0.5f, 1.f, 0.4f));
	}
	else
	{
		selected = false;
		model->setTint(glm::vec4(ownerColor, 0.3f));
	}
}

bool Bomb::hasNewHeading() const
{
	return newHeading;
}

void Bomb::setHasNewHeading(bool newHead)
{
	newHeading = newHead;
}

bool Bomb::setTarget(Map const& map, glm::vec3 const& targetPos)
{
	bool result = map.findPath(getPosition(), targetPos, path);

	if (!result)
	{
		setVelocity(glm::vec3(0.f));
	}

	return result;
}

void Bomb::halt()
{
	path.clear();
	setVelocity(glm::vec3(0.f));
}

bool Bomb::rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const
{
	return model->rayIntersect(origin, direction, distance);
}

bool Bomb::frustumIntersect(Frustum const& frustum) const
{
	return model->frustumIntersect(frustum);
}

void Bomb::updatePosition(float deltaTime)
{
	if (!path.empty())
	{
		const static float speed = 3.f;
		glm::vec3 pos = getPosition();
		glm::vec3 direction = glm::vec3();
		float distanceToGo = speed * deltaTime;

		while (distanceToGo > 0 && !path.empty())
		{
			float distance = glm::distance(pos, path.front());

			if (distanceToGo >= distance)
			{
				distanceToGo -= distance;
				pos = path.front();
				path.pop_front();
				newHeading = true;
			}
			else
			{
				direction = glm::normalize(path.front() - pos);
				pos += direction * distanceToGo;
				model->setRotation(glm::vec3(0, glm::degrees(glm::atan(-direction.z, direction.x)), 0));
				distanceToGo = 0;
			}
		}

		setPosition(pos);

		if (newHeading)
		{
			setVelocity(direction * speed);
		}
	}
	else
	{
		glm::vec3 pos = getPosition() + getVelocity() * deltaTime;
		setPosition(pos);
	}
}

void Bomb::draw(Graphics::ptr graphics)
{
	graphics->drawModel(model);
}
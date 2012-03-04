#include "Bomb.h"

#include "StandardBombModelData.h"

Bomb::Bomb(int team)
	: team(team), selected(false), model(new Model(StandardBombModelData::getInstance()))
{
	model->setScale(glm::vec3(0.3f));
}

glm::vec3 const& Bomb::getPosition() const
{
	return model->getPosition();
}

void Bomb::setPosition(glm::vec3 const& pos)
{
	model->setPosition(pos);
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
		model->setTint(glm::vec4(0.f));
	}
}

bool Bomb::setTarget(Map const& map, glm::vec3 const& targetPos)
{
	return map.findPath(getPosition(), targetPos, path);
}

void Bomb::halt()
{
	path.clear();
}

bool Bomb::rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance)
{
	return model->rayIntersect(origin, direction, distance);
}

void Bomb::updatePosition(float deltaTime)
{
	if (!path.empty())
	{
		const static float speed = 3.f;
		glm::vec3 pos = getPosition();
		float distanceToGo = speed * deltaTime;

		while (distanceToGo > 0 && !path.empty())
		{
			float distance = glm::distance(pos, path.front());

			if (distanceToGo >= distance)
			{
				distanceToGo -= distance;
				pos = path.front();
				path.pop_front();
			}
			else
			{
				glm::vec3 direction = glm::normalize(path.front() - pos);
				pos += direction * distanceToGo;
				model->setRotation(glm::vec3(0, glm::degrees(glm::atan(-direction.z, direction.x)), 0));
				distanceToGo = 0;
			}
		}

		setPosition(pos);
	}
}

void Bomb::draw(Graphics::ptr graphics)
{
	graphics->drawModel(model);
}
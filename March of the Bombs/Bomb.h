#pragma once

#include <list>

#include "Model.h"
#include "MapHeader.h"

class Bomb 
{
private:
	Model::ptr model;

	unsigned short owner;
	bool selected;
	bool newHeading;

	glm::vec3 velocity;

	std::list<glm::vec3> path;

public:
	Bomb();
	Bomb(unsigned short owner);

	unsigned short getOwner() const;

	glm::vec3 const& getPosition() const;
	glm::vec3 const& getRotation() const;
	glm::vec3 const& getVelocity() const;

	void setPosition(glm::vec3 const& pos);
	void setRotation(glm::vec3 const& rot);
	void setVelocity(glm::vec3 const& vel);

	bool isSelected() const;
	void setSelected(bool select);

	bool hasNewHeading() const;
	void setHasNewHeading(bool newHead);

	bool setTarget(Map const& map, glm::vec3 const& targetPos);
	void halt();

	bool rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance);

	void updatePosition(float deltaTime);

	void draw(Graphics::ptr graphics);
};
#pragma once

#include <list>

#include "Model.h"
#include "MapHeader.h"

class Bomb 
{
private:
	Model::ptr model;

	int team;
	bool selected;

	std::list<glm::vec3> path;

public:
	Bomb(int team);

	int getTeam();

	glm::vec3 const& getPosition() const;
	void setPosition(glm::vec3 const& pos);

	bool isSelected() const;
	void setSelected(bool select);

	bool setTarget(Map const& map, glm::vec3 const& targetPos);
	void halt();

	bool rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance);

	void updatePosition(float deltaTime);

	void draw(Graphics::ptr graphics);
};
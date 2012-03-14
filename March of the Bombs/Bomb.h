#pragma once

#include <list>
#include <set>

#include "Model.h"
#include "Graphics.h"

class Map;

/**
 * Bomb represents a single bomb on the map and some of its properties.
 */
class Bomb 
{
private:
	/// The model this bomb uses
	Model::ptr model;

	/// ID of the owning player
	unsigned short owner;
	/// Selected bomb are drawn with a blue tint
	bool selected;
	/// The selected bomb has recently altered direction/velocity/position
	bool newHeading;

	/// Last known velocity of the bomb
	glm::vec3 velocity;

	/// The path the bomb is currently following
	std::list<glm::vec3> path;

public:
	/// Bomb id - first is playerID, second is entityID
	typedef std::pair<unsigned short, unsigned short> id;
	typedef std::set<id> id_set;

	/// Constructor.
	Bomb();
	/**
	 * Constructor. Assigns the owner of the bomb, loads the standard bomb model
	 * and sets some default values.
	 */
	Bomb(unsigned short owner);

	/// Return the bombs owner
	unsigned short getOwner() const;

	/// Return the current position
	glm::vec3 const& getPosition() const;
	/// Return the current rotation
	glm::vec3 const& getRotation() const;
	/// Return the current velocity
	glm::vec3 const& getVelocity() const;

	/// Set the current position
	void setPosition(glm::vec3 const& pos);
	/// Set the current rotation
	void setRotation(glm::vec3 const& rot);
	/// Set the current velocity
	void setVelocity(glm::vec3 const& vel);

	/// Return whether the bomb is currently selected
	bool isSelected() const;
	/// Set whether the bomb is selected or not
	void setSelected(bool select);

	/// Return whether the bomb has a new heading
	bool hasNewHeading() const;
	/// Set whether the bomb has a new heading or not
	void setHasNewHeading(bool newHead);

	/**
	 * Calculate a new path for the bomb that should reach targetPos.
	 * If targetPos can not be reached, the bomb will stop.
	 */
	bool setTarget(Map const& map, glm::vec3 const& targetPos);
	/// Stop the bomb at its current position
	void halt();

	/**
	 * Calculate whether the bombs bounding box intersects the ray that goes from origin in the direction
	 * direction, at a point closer than distance along the direction. If so, distance is updated to the
	 * distance to the intersection with the bomb.
	 */
	bool rayIntersect(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const;
	/**
	 * Calculate whether the bombs bounding box intersects a frustum.
	 */
	bool frustumIntersect(Frustum const& frustum) const;

	/// Update the current position of the bomb
	void updatePosition(float deltaTime);

	/// Draw the bomb on the screen
	void draw(Graphics::ptr graphics);
};
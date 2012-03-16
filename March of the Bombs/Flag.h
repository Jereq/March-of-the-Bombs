#pragma once

#include "Block.h"
#include "FlagModelData.h"

class Flag :
	public Block
{
private:
	int ownerID;

public:
	// constructor
	Flag(glm::vec3 const& position);

	/// Return the flags position
	glm::vec3 getPosition() const;

	/// Get the id of the player who owns the flag, or -1 if no one owns it
	int getOwner() const;
	/// Set the new owner of the flag and the color
	void setOwner(int ownerID, glm::vec3 const& color);
};
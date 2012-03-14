#pragma once

#include "Block.h"

/**
 * SoftBlock represents a block that can be destroyed by an explosion.
 */
class SoftBlock :
	public Block
{
public:
	/// Constructor. Loads the soft block model.
	SoftBlock(glm::vec3 const& position);

	/// Return true, as this is a destructible block
	virtual bool isDestructible() const;
};
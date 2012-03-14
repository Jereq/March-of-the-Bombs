#pragma once

#include "Block.h"

/**
 * A hard block that cannot be destroyed.
 */
class HardBlock :
	public Block
{
public:
	/// Constructor. Sets the models position.
	HardBlock(glm::vec3 const& position);
};
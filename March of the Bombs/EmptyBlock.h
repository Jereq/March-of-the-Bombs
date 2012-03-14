#pragma once

#include "Block.h"

/**
 * A block of air that is pretty useless.
 */
class EmptyBlock :
	public Block
{
public:
	/// Constructor. Creates a block without a model.
	EmptyBlock();
};


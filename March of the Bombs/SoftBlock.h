#pragma once

#include "Block.h"

class SoftBlock :
	public Block
{
public:
	SoftBlock(glm::vec3 const& position);
	~SoftBlock();
};
#pragma once

#include "Block.h"

class HardBlock :
	public Block
{
public:
	HardBlock(glm::vec3 const& position);
	~HardBlock();
};
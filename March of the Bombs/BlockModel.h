#pragma once

#include "Model.h"

class BlockModel :
	public Model
{
public:
	typedef boost::shared_ptr<BlockModel> ptr;

	BlockModel();
};
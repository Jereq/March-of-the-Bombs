#pragma once

#include <boost\shared_ptr.hpp>

#include "Model.h"
#include "BlockModelData.h"
#include "Graphics.h"

class Block
{
protected:
	Model::ptr model;

	Block(Model::ptr const& model);

public:
	typedef boost::shared_ptr<Block> ptr;

	~Block();

	virtual void update(float deltaTime);
	virtual void draw(Graphics::ptr graphics);
};
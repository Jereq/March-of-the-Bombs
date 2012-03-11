#include "Block.h"

Block::Block(Model::ptr const& model)
	: model(model)
{
}

Block::~Block()
{
}

void Block::update(float deltaTime)
{
}

void Block::draw(Graphics::ptr graphics)
{
	if (model)
	{
		graphics->drawModel(model);
	}
}

bool Block::isDestructible() const
{
	return false;
}
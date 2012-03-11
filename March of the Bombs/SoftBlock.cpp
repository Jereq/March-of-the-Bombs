#include "SoftBlock.h"

SoftBlock::SoftBlock(glm::vec3 const& position)
	: Block(Model::ptr(new Model(BlockModelData::getSoftInstance())))
{
	model->setPosition(position);
}

SoftBlock::~SoftBlock()
{
}

bool SoftBlock::isDestructible() const
{
	return true;
}
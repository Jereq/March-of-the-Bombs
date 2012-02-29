#include "HardBlock.h"

HardBlock::HardBlock(glm::vec3 const& position)
	: Block(Model::ptr(new Model(BlockModelData::getHardInstance())))
{
	model->setPosition(position);
}

HardBlock::~HardBlock()
{
}
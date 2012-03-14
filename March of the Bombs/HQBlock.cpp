#include "HQBlock.h"



HQBlock::HQBlock(unsigned short owner, glm::vec3 const& position)
	: ownerAtTheMax(ownerAtTheMax), Block(Model::ptr(new Model(HQModelData::getInstance())))
{
	model->setScale(glm::vec3(0.3f));
	model->setPosition(position);
}

HQBlock::~HQBlock()
{
}

//void HQBlock::drawHQ(Graphics::ptr graphics)
//{
//	graphics->drawModel(model);
//}
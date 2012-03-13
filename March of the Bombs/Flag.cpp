#include "Flag.h"

Flag::Flag(glm::vec3 const& position)
	: Block(Model::ptr(new Model(FlagModelData::getInstance())))
{
	model->setPosition(position);
}
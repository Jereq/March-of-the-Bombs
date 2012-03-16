#include "Flag.h"

Flag::Flag(glm::vec3 const& position)
	: Block(Model::ptr(new Model(FlagModelData::getInstance()))), ownerID(-1)
{
	model->setPosition(position + glm::vec3(0.5f, 0, 0.5f));
}

glm::vec3 Flag::getPosition() const
{
	return model->getPosition();
}

int Flag::getOwner() const
{
	return ownerID;
}

void Flag::setOwner(int ownerID, glm::vec3 const& color)
{
	this->ownerID = ownerID;
	if (ownerID < 0)
	{
		model->setTint(glm::vec4(0));
	}
	else
	{
		model->setTint(glm::vec4(color, 0.2f));
	}
}
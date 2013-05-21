#include "LoudEntity.h"

#include "StandardBombModelData.h"

LoudEntity::LoudEntity(SoundManager* soundManager, std::string const& soundFilename)
	: model(new Model(StandardBombModelData::getInstance()))
{
	model->setScale(glm::vec3(0.3f));

	sound = soundManager->playSound(soundFilename, model->getPosition(), 5.f, true);
}

LoudEntity::~LoudEntity()
{
	if (sound)
	{
		delete sound;
	}
}

void LoudEntity::setPosition(glm::vec3 const& pos)
{
	model->setPosition(pos);
	sound->setPosition(pos);
}

glm::vec3 LoudEntity::getPosition() const
{
	return model->getPosition();
}

void LoudEntity::setDirection(glm::vec3 const& dir)
{
	float yRot = glm::degrees(glm::atan(-dir.z, dir.x));
	model->setRotation(glm::vec3(0.f, yRot, 0.f));
}

void LoudEntity::draw(Graphics::ptr graphics)
{
	graphics->drawModel(model);
}

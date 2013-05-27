#include "LoudEntity.h"

#include "StandardBombModelData.h"

LoudEntity::LoudEntity(SoundManager* soundManager, std::string const& soundFilename)
	: model(new Model(StandardBombModelData::getInstance()))
{
	// Scale down
	model->setScale(glm::vec3(0.3f));

	// Create channel and start playing looping sound at the position of the model.
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
	// Get angle around y-axis
	float yRot = glm::degrees(glm::atan(-dir.z, dir.x));
	model->setRotation(glm::vec3(0.f, yRot, 0.f));
}

void LoudEntity::setVolume(float volume)
{
	sound->setVolume(volume);
}

void LoudEntity::draw(Graphics::ptr graphics)
{
	graphics->drawModel(model);
}

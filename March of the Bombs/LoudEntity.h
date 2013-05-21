#pragma once

#include "Graphics.h"
#include "MChannel.h"
#include "Model.h"
#include "SoundManager.h"

class LoudEntity
{
private:
	MChannel* sound;
	Model::ptr model;

public:
	LoudEntity(SoundManager* soundManager, std::string const& soundFilename);
	~LoudEntity();

	void setPosition(glm::vec3 const& pos);
	glm::vec3 getPosition() const;

	void setDirection(glm::vec3 const& dir);

	void draw(Graphics::ptr graphics);
};

#pragma once

#include "Graphics.h"
#include "MChannel.h"
#include "Model.h"
#include "SoundManager.h"

/*
 * Test class containing a bomb model and a sound channel playing a looping sound.
 */
class LoudEntity
{
private:
	MChannel* sound;
	Model::ptr model;

public:
	/*
	 * constructor.
	 *
	 * Creates a bomb entity with a looping sound playing at the same position.
	 */
	LoudEntity(SoundManager* soundManager, std::string const& soundFilename);
	~LoudEntity();

	/*
	 * Change the position of the entity.
	 * Also changes the position of the sound associated with the entity.
	 */
	void setPosition(glm::vec3 const& pos);
	/*
	 * Get the position of the entity.
	 */
	glm::vec3 getPosition() const;

	/*
	 * Set the direction the entity is facing.
	 */
	void setDirection(glm::vec3 const& dir);
	/*
	 * Get the direction the entity is currently facing.
	 */
	void setVolume(float volume);

	/*
	 * Queues the entity to be drawn by the graphics object.
	 */
	void draw(Graphics::ptr graphics);
};

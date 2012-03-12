#pragma once

#include <glm/glm.hpp>

#include "TextureSection.h"

class Animation
{
private:
	float totalTime;
	float currentTime;
	glm::ivec2 currentFrame;
	bool loop;
	bool done;

	glm::ivec2 numFrames;
	glm::vec2 frameSize;

	TextureSection aniTexture;

public:
	Animation();
	Animation(TextureSection const& aniTexture, glm::ivec2 const& numFrames, float totalTime, bool loop);

	void update(float deltaTime);

	float getCurrentTime() const;
	float getTotalTime() const;
	bool isLooping() const;
	bool isDone() const;

	glm::ivec2 const& getNumFrames() const;
	
	TextureSection getFrame() const;
};
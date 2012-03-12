#include "Animation.h"

Animation::Animation()
	: done(true), loop(false)
{
}

Animation::Animation(TextureSection const& aniTexture, glm::ivec2 const& numFrames, float totalTime, bool loop)
	: aniTexture(aniTexture), numFrames(numFrames), totalTime(totalTime), loop(loop),
	currentTime(0), currentFrame(0, 0), done(false), frameSize(1.f / numFrames.x, 1.f / numFrames.y)
{
}

void Animation::update(float deltaTime)
{
	if (done)
	{
		return;
	}

	currentTime += deltaTime;

	if (currentTime > totalTime)
	{
		if (loop)
		{
			currentTime -= totalTime;
		}
		else
		{
			done = true;
			return;
		}
	}

	size_t nFrames = numFrames.x * numFrames.y;
	size_t thisFrame = static_cast<size_t>(currentTime / totalTime * nFrames);

	currentFrame.x = thisFrame % numFrames.x;
	currentFrame.y = numFrames.y - thisFrame / numFrames.x - 1;
}

float Animation::getCurrentTime() const
{
	return currentTime;
}

float Animation::getTotalTime() const
{
	return totalTime;
}

bool Animation::isLooping() const
{
	return loop;
}

bool Animation::isDone() const
{
	return done;
}

glm::ivec2 const& Animation::getNumFrames() const
{
	return numFrames;
}

TextureSection Animation::getFrame() const
{
	return aniTexture.cut(Rectanglef(glm::vec2(currentFrame.x * frameSize.x, currentFrame.y * frameSize.y), frameSize));
}
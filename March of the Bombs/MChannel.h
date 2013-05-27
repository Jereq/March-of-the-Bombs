#pragma once

#include <fmod/fmod.hpp>
#include <glm/glm.hpp>

#include <vector>

#include "SoundManager.h"

class SoundManager;

/*
 * MChannel is a wrapper around FMOD::Channel together with a FMOD::DSP
 * used to play a sound, optionally in 3D, and optionally with a filter.
 * Most of the member variables duplicate functionality in FMOD::Channel.
 */
class MChannel
{
private:
	SoundManager* soundManager;

	FMOD::Channel* channel;
	FMOD::DSP* dsp;
	FMOD::Sound* sound;

	glm::vec3 position;
	float minDistance;
	float maxDistance;
	bool isPaused;
	bool isDone;
	float volume;
	unsigned int PCM_Pos;

	std::vector<float> echoBuffer;
	unsigned int echoBufferPos;
	float prevLowPassLeft;
	float prevLowPassRight;

	// Remove unwanted possibilities
	MChannel(MChannel const&);
	MChannel & operator= (MChannel const&);

	static void errCheck(FMOD_RESULT const& result);

	// Passes calls to DSP_Callback
	static FMOD_RESULT F_CALLBACK staticDSP_Callback(
		FMOD_DSP_STATE *  dsp_state, 
		float *  inbuffer, 
		float *  outbuffer, 
		unsigned int  length, 
		int  inchannels, 
		int  outchannels
	);

	/*
	 * Sound generation function.
	 */
	FMOD_RESULT DSP_Callback(
		FMOD_DSP_STATE *  dsp_state, 
		float *  inbuffer, 
		float *  outbuffer, 
		unsigned int  length, 
		int  inchannels, 
		int  outchannels
	);

	glm::vec2 calculate3DPan();
	void processPart(int16_t* ptr1, unsigned int len1, SoundManager::FilterType filterType, float* outBuffer, unsigned int& outIndex, glm::vec2 pan);

public:
	/*
	 * constructor.
	 * Creates and plays a DSP with the DSP_Callback used for the read callback.
	 */
	MChannel(FMOD::System* system, FMOD::Sound* sound, SoundManager* manager);
	~MChannel();

	/*
	 * Set the 3D position to be used to calculate stereo panning.
	 * Requires the sound to be 3D to have any effect.
	 */
	void setPosition(glm::vec3 const& position);
	/*
	 * Get the current 3D position used for this channel.
	 */
	glm::vec3 getPosition() const;

	/*
	 * Set the minimum and maximum distances for the channel.
	 * Listener positions closer than min from the channels position yields full volume,
	 * while positions farther away than max yields no volume.
	 * Positions inbetween yields volumes depending on the distance relative to min.
	 *
	 * Requires the sound to be 3D to have any effect.
	 */
	void setMinMaxDistance(float min, float max);
	/*
	 * Get the minimum distance for distance related volume control.
	 * Smaller distances yields full volume.
	 */
	float getMinDistance() const;
	/*
	 * Get the maximum distance for distance related volume control.
	 * Greater distances yields no volume.
	 */
	float getMaxDistance() const;

	/*
	 * Pause or unpause the channel.
	 */
	void setPaused(bool paused);
	/*
	 * Get whether the channel is paused or not.
	 */
	bool getIsPaused() const;

	/*
	 * Get whether the channel has reached the end of the sound and is not looping.
	 */
	bool getIsDone() const;

	/*
	 * Set the master volume for this channel. Applies to all sounds.
	 */
	void setVolume(float value);
	/*
	 * Get the current volume of this channel.
	 */
	float getVolume() const;
};

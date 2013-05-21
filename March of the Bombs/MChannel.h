#pragma once

#include <fmod/fmod.hpp>
#include <glm/glm.hpp>

class SoundManager;

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

	MChannel(MChannel const&);
	MChannel & operator= (MChannel const&);

	static void errCheck(FMOD_RESULT const& result);

	static FMOD_RESULT F_CALLBACK staticDSP_Callback(
		FMOD_DSP_STATE *  dsp_state, 
		float *  inbuffer, 
		float *  outbuffer, 
		unsigned int  length, 
		int  inchannels, 
		int  outchannels
	);

	FMOD_RESULT DSP_Callback(
		FMOD_DSP_STATE *  dsp_state, 
		float *  inbuffer, 
		float *  outbuffer, 
		unsigned int  length, 
		int  inchannels, 
		int  outchannels
	);

public:
	MChannel(FMOD::System* system, FMOD::Sound* sound, SoundManager* manager);
	~MChannel();

	void setPosition(glm::vec3 const& position);
	glm::vec3 getPosition() const;

	void setMinMaxDistance(float min, float max);
	float getMinDistance() const;
	float getMaxDistance() const;

	void setPaused(bool paused);
	bool getIsPaused() const;

	bool getIsDone() const;

	void setVolume(float value);
	float getVolume() const;
};

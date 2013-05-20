#include "MChannel.h"

#include <sstream>
#include <stdint.h>

#include <fmod/fmod_errors.h>

void MChannel::errCheck(FMOD_RESULT const& result)
{
    if (result != FMOD_OK)
    {
		std::ostringstream ostr;
		ostr << "FMOD error ! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		throw std::exception(ostr.str().c_str());
    }
}

FMOD_RESULT F_CALLBACK MChannel::staticDSP_Callback(
		FMOD_DSP_STATE *  dsp_state, 
		float *  inbuffer, 
		float *  outbuffer, 
		unsigned int  length, 
		int  inchannels, 
		int  outchannels)
{
	MChannel* userData;
	FMOD::DSP* dspState = (FMOD::DSP*)dsp_state->instance;

	FMOD_RESULT result = dspState->getUserData((void**)&userData);
	errCheck(result);

	return userData->DSP_Callback(dsp_state, inbuffer, outbuffer, length, inchannels, outchannels);
}

FMOD_RESULT MChannel::DSP_Callback(
		FMOD_DSP_STATE *  dsp_state, 
		float *  inbuffer, 
		float *  outbuffer, 
		unsigned int  length, 
		int  inchannels, 
		int  outchannels)
{
	unsigned int outIndex = 0;

	int16_t *ptr1, *ptr2;
	unsigned int len1, len2;

	sound->lock(PCM_Pos, length * outchannels * sizeof(int16_t), (void**)&ptr1, (void**)&ptr2, &len1, &len2);

	for (unsigned int i = 0; i < len1 / sizeof(int16_t); i++)
	{
		outbuffer[outIndex++] = (float)ptr1[i] / std::numeric_limits<int16_t>::max() * volume;
	}

	for (unsigned int i = 0; i < len2 / sizeof(int16_t); i++)
	{
		outbuffer[outIndex++] = (float)ptr2[i] / std::numeric_limits<int16_t>::max() * volume;
	}

	sound->unlock(ptr1, ptr2, len1, len2);

	if (len2)
	{
		PCM_Pos = len2;
	}
	else
	{
		PCM_Pos += len1;
	}

	return FMOD_OK;
}

MChannel::MChannel(FMOD::System* system, FMOD::Sound* sound)
	: position(0.f, 0.f, 0.f)
	, minDistance(0.f)
	, maxDistance(10000.f)
	, isPaused(false)
	, volume(1.f)
	, PCM_Pos(0)
{
	FMOD_RESULT result;

	FMOD_DSP_DESCRIPTION dspDesc =
	{
		"SoundManagerAwesomeness",
		1,
		2,
		nullptr,
		nullptr,
		nullptr,
		&staticDSP_Callback,
		nullptr,
		0,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		0,
		this
	};

	this->sound = sound;

	result = system->createDSP(&dspDesc, &dsp);
	errCheck(result);

	result = system->playDSP(FMOD_CHANNEL_FREE, dsp, true, &channel);
	errCheck(result);
}

MChannel::~MChannel()
{
	errCheck(dsp->release());
}

void MChannel::setPosition(glm::vec3 const& position)
{
	this->position = position;
}

glm::vec3 MChannel::getPosition() const
{
	return position;
}

void MChannel::setMinMaxDistance(float min, float max)
{
	minDistance = min;
	maxDistance = max;
}

float MChannel::getMinDistance() const
{
	return minDistance;
}

float MChannel::getMaxDistance() const
{
	return maxDistance;
}

void MChannel::setPaused(bool paused)
{
	channel->setPaused(paused);

	isPaused = paused;
}

bool MChannel::getIsPaused() const
{
	return isPaused;
}

void MChannel::setVolume(float value)
{
	volume = value;
}

float MChannel::getVolume() const
{
	return volume;
}
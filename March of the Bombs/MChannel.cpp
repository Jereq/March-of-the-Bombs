#include "MChannel.h"

#include "SoundManager.h"

#include <sstream>
#include <stdint.h>

#include <fmod/fmod_errors.h>
#include <glm/gtc/swizzle.hpp>

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
	if (isPaused || isDone)
	{
		return FMOD_ERR_NOTREADY;
	}

	FMOD_RESULT result;

	unsigned int outIndex = 0;

	int16_t *ptr1, *ptr2;
	unsigned int len1, len2;
	
	result = sound->lock(PCM_Pos, length * outchannels * sizeof(int16_t), (void**)&ptr1, (void**)&ptr2, &len1, &len2);
	errCheck(result);

	FMOD_MODE mode;
	result = sound->getMode(&mode);
	errCheck(result);

	bool looping = (mode & FMOD_LOOP_NORMAL) != 0;

	float leftVolume = 1.f;
	float rightVolume = 1.f;
	if (mode & FMOD_3D)
	{
		const static float STATIC_SCALE(glm::sqrt(2.f) * 0.5f);

		glm::vec3 listenerPos = soundManager->getListenerPosition();
		glm::mat3 invBaseMat = soundManager->getInverseBaseMatrix();
		glm::vec3 relPos = position - listenerPos;

		glm::vec3 transformedPos = invBaseMat * relPos;
		
		float lengthInPlane = glm::length(glm::swizzle<glm::X, glm::Z>(transformedPos)());

		if (lengthInPlane != 0.f)
		{
			float cosAngle = -transformedPos.z / lengthInPlane;
			float sinAngle = glm::sqrt((1.f - cosAngle) * 0.5f);
			cosAngle = glm::sqrt((1.f + cosAngle) * 0.5f);

			if (transformedPos.x < 0.f)
			{
				sinAngle *= -1.f;
			}

			leftVolume = STATIC_SCALE * (cosAngle - sinAngle);
			rightVolume = STATIC_SCALE * (cosAngle + sinAngle);
		}
		else
		{
			leftVolume = STATIC_SCALE;
			rightVolume = STATIC_SCALE;
		}

		float distVolume = 1.f;
		float distance = glm::length(relPos);
		if (distance <= minDistance)
		{
			distVolume = 1.f;
		}
		else if (distance >= maxDistance)
		{
			distVolume = 0.f;
		}
		else
		{
			distVolume = minDistance / distance;
			distVolume *= distVolume;
		}

		leftVolume *= distVolume;
		rightVolume *= distVolume;
	}

	for (unsigned int i = 0; i < len1 / sizeof(int16_t); i += 2)
	{
		outbuffer[outIndex++] = (float)ptr1[i    ] / std::numeric_limits<int16_t>::max() * volume * leftVolume;
		outbuffer[outIndex++] = (float)ptr1[i + 1] / std::numeric_limits<int16_t>::max() * volume * rightVolume;
	}

	if (looping)
	{
		for (unsigned int i = 0; i < len2 / sizeof(int16_t); i += 2)
		{
			outbuffer[outIndex++] = (float)ptr2[i    ] / std::numeric_limits<int16_t>::max() * volume * leftVolume;
			outbuffer[outIndex++] = (float)ptr2[i + 1] / std::numeric_limits<int16_t>::max() * volume * rightVolume;
		}
	}

	result = sound->unlock(ptr1, ptr2, len1, len2);
	errCheck(result);

	if (looping)
	{
		if (len2)
		{
			PCM_Pos = len2;
		}
		else
		{
			PCM_Pos += len1;
	
			unsigned int PCM_Length;
			sound->getLength(&PCM_Length, FMOD_TIMEUNIT_PCMBYTES);

			if (PCM_Pos >= PCM_Length)
			{
				PCM_Pos = 0;
			}
		}
	}
	else
	{
		PCM_Pos += len1;
	
		unsigned int PCM_Length;
		sound->getLength(&PCM_Length, FMOD_TIMEUNIT_PCMBYTES);

		if (PCM_Pos >= PCM_Length)
		{
			PCM_Pos = 0;
			isDone = true;
			isPaused = true;

			return FMOD_ERR_NOTREADY;
		}
	}

	return FMOD_OK;
}

MChannel::MChannel(FMOD::System* system, FMOD::Sound* sound, SoundManager* manager)
	: position(0.f, 0.f, 0.f)
	, minDistance(0.f)
	, maxDistance(10000.f)
	, isPaused(false)
	, isDone(false)
	, volume(1.f)
	, PCM_Pos(0)
	, soundManager(manager)
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
	FMOD::System* system;
	FMOD_RESULT result = dsp->getSystemObject(&system);
	errCheck(result);

	result = system->lockDSP();
	errCheck(result);

	result = dsp->release();
	errCheck(result);

	result = system->unlockDSP();
	errCheck(result);
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

bool MChannel::getIsDone() const
{
	return isDone;
}

void MChannel::setVolume(float value)
{
	volume = value;
}

float MChannel::getVolume() const
{
	return volume;
}
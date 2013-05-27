#include "MChannel.h"

#include "SoundManager.h"

#include <sstream>
#include <stdint.h>

#include <fmod/fmod_errors.h>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtx/constants.hpp>

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
	// Get DSP state
	FMOD::DSP* dspState = (FMOD::DSP*)dsp_state->instance;

	// Get MChannel object from DSP state
	MChannel* userData;
	FMOD_RESULT result = dspState->getUserData((void**)&userData);
	errCheck(result);

	// Chain function call to object.
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
	// Skip while paused or done
	if (isPaused || isDone)
	{
		return FMOD_ERR_NOTREADY;
	}

	FMOD_RESULT result;

	unsigned int outIndex = 0;

	int16_t *ptr1, *ptr2;
	unsigned int len1, len2;
	
	// Get the next data from the sound (Assuming 44100 Hz, 16bit sound, stereo, works for our sounds)
	result = sound->lock(PCM_Pos, length * outchannels * sizeof(int16_t), (void**)&ptr1, (void**)&ptr2, &len1, &len2);
	errCheck(result);

	// Get sound mode
	FMOD_MODE mode;
	result = sound->getMode(&mode);
	errCheck(result);

	// Check if the sound should loop
	bool looping = (mode & FMOD_LOOP_NORMAL) != 0;

	// Get the filter type to use
	SoundManager::FilterType filterType = soundManager->getFilterType();

	// Calculate pan for 3D
	glm::vec2 pan(1.f, 1.f);
	if (mode & FMOD_3D)
	{
		pan = calculate3DPan();
	}

	// Process input into output
	processPart(ptr1, len1, filterType, outbuffer, outIndex, pan);
	if (looping)
	{
		processPart(ptr2, len2, filterType, outbuffer, outIndex, pan);
	}

	// Signal done with data
	result = sound->unlock(ptr1, ptr2, len1, len2);
	errCheck(result);

	// Rewind, if needed
	if (looping)
	{
		// Check if we wrapped around
		if (len2)
		{
			PCM_Pos = len2;
		}
		else
		{
			// Move forward
			PCM_Pos += len1;
	
			// To be extra sure, get length of sound and check for end
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
		// We're not looping, so move forward and check for end
		PCM_Pos += len1;
	
		unsigned int PCM_Length;
		sound->getLength(&PCM_Length, FMOD_TIMEUNIT_PCMBYTES);

		// Done, stop the processing
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

glm::vec2 MChannel::calculate3DPan()
{
	const static float STATIC_SCALE(glm::sqrt(2.f) * 0.5f);

	// Get listener values
	glm::vec3 listenerPos = soundManager->getListenerPosition();
	glm::mat3 invBaseMat = soundManager->getInverseBaseMatrix();

	// Calculate position relative to listener
	glm::vec3 relPos = position - listenerPos;
	glm::vec3 transformedPos = invBaseMat * relPos;
		
	// Cacluclate the distance in the xz-plane
	float lengthInPlane = glm::length(glm::swizzle<glm::X, glm::Z>(transformedPos)());

	glm::vec2 pan(1.f);

	// Check for positions right above or right below the listener
	if (lengthInPlane != 0.f)
	{
		// No need for the actual angle, cos and sine will do. Also halves the angles.
		float cosAngle = -transformedPos.z / lengthInPlane;
		float sinAngle = glm::sqrt((1.f - cosAngle) * 0.5f);
		cosAngle = glm::sqrt((1.f + cosAngle) * 0.5f);

		// Mirror sine value to correct side if left
		if (transformedPos.x < 0.f)
		{
			sinAngle *= -1.f;
		}

		// Constant power stereo panning
		pan.x = STATIC_SCALE * (cosAngle - sinAngle);
		pan.y = STATIC_SCALE * (cosAngle + sinAngle);
	}
	else
	{
		// "Full" volume in both left and right for positions above or below the listener.
		pan.x = STATIC_SCALE;
		pan.y = STATIC_SCALE;
	}

	// Get the distance of the sound from the listener
	float distVolume = 1.f;
	float distance = glm::length(relPos);

	// Check for min and max distance
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
		// Divide volume by distance squared, relative to minDistance
		distVolume = minDistance / distance;
		distVolume *= distVolume;
	}

	// Apply distance volume control
	pan.x *= distVolume;
	pan.y *= distVolume;

	return pan;
}

void MChannel::processPart(int16_t* inBuffer, unsigned int inLength, SoundManager::FilterType filterType, float* outBuffer, unsigned int& outIndex, glm::vec2 pan)
{
	for (unsigned int i = 0; i < inLength / sizeof(int16_t); i += 2)
	{
		// Get sample frame from in buffer and convert to float
		float outLeft =  (float)inBuffer[i    ] / std::numeric_limits<int16_t>::max() * volume;
		float outRight = (float)inBuffer[i + 1] / std::numeric_limits<int16_t>::max() * volume;

		const static float CUTOFF_FREQUENCY = 500.f;	// Cutoff for low-pass filter
		const static float SAMPLE_RATE = 44100.f;		// Assumed sample rate (works for the selected sound files, mostly)

		// Low-pass filter constants
		static float LOW_PASS_B1 = 2.f - glm::cos(2.f * glm::pi<float>() * CUTOFF_FREQUENCY / SAMPLE_RATE);
		static float LOW_PASS_B = glm::sqrt(LOW_PASS_B1 * LOW_PASS_B1 - 1.f) - LOW_PASS_B1; 
		static float LOW_PASS_A = 1.f + LOW_PASS_B;

		// Calculate low-pass values
		prevLowPassLeft = LOW_PASS_A * outLeft - LOW_PASS_B * prevLowPassLeft;
		prevLowPassRight = LOW_PASS_A * outRight - LOW_PASS_B * prevLowPassRight;

		// Scale of echo
		const static float DOWN_SCALE = 0.5f;

		// Caclulate new echo values
		echoBuffer[echoBufferPos    ] = echoBuffer[echoBufferPos    ] * DOWN_SCALE + outLeft;
		echoBuffer[echoBufferPos + 1] = echoBuffer[echoBufferPos + 1] * DOWN_SCALE + outRight;

		// Apply filter, if selected
		switch (filterType)
		{
		default:
		case SoundManager::FilterType::NORMAL:	// No filter
			outBuffer[outIndex++] = outLeft * pan.x;
			outBuffer[outIndex++] = outRight * pan.y;
			break;
			
		case SoundManager::FilterType::ECHO:
			outBuffer[outIndex++] = echoBuffer[echoBufferPos    ] * pan.x;
			outBuffer[outIndex++] = echoBuffer[echoBufferPos + 1] * pan.y;
			break;
			
		case SoundManager::FilterType::DAMPENING:
			outBuffer[outIndex++] = prevLowPassLeft * pan.x;
			outBuffer[outIndex++] = prevLowPassRight * pan.y;
			break;
		}

		// Move and wrap around the echo buffer
		echoBufferPos += 2;
		if (echoBufferPos >= echoBuffer.size())
		{
			echoBufferPos = 0;
		}
	}
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
	, sound(sound)
	, echoBufferPos(0)
	, prevLowPassLeft(0)
	, prevLowPassRight(0)
{
	FMOD_RESULT result;

	const static unsigned int NUM_CHANNELS = 2;

	// Get the frequency (sample rate) of the sound. We later assume it to be 44100 Hz.
	float frequency;
	result = sound->getDefaults(&frequency, nullptr, nullptr, nullptr);
	echoBuffer.resize((unsigned int)(0.5f * frequency) * NUM_CHANNELS);

	if (frequency != 44100.f)
	{
		char name[256];
		result = sound->getName(name, 256);
		errCheck(result);

		printf("Sound with strange sample rate used! (%s, %0.f Hz)\n", name, frequency);
	}

	// Setup DSP
	FMOD_DSP_DESCRIPTION dspDesc =
	{
		"SoundManagerAwesomeness",
		1,
		NUM_CHANNELS,
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

	// Create the needed DSP
	result = system->createDSP(&dspDesc, &dsp);
	errCheck(result);

	// Add DSP to the system, but paused
	result = system->playDSP(FMOD_CHANNEL_FREE, dsp, true, &channel);
	errCheck(result);
}

MChannel::~MChannel()
{
	// Get the system of the DSP
	FMOD::System* system;
	FMOD_RESULT result = dsp->getSystemObject(&system);
	errCheck(result);

	// Lock will block if until the DSP is no longer running. Needed to prevent the sound from misbehaving.
	result = system->lockDSP();
	errCheck(result);

	// Get rid of the dsp, stops this channel from playing.
	result = dsp->release();
	errCheck(result);

	// Unlock DSP system to allow sound to be generated again
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
#include "SoundManager.h"

#include <exception>
#include <sstream>

#include <boost/foreach.hpp>

FMOD::System* SoundManager::system(NULL);

void SoundManager::errCheck(FMOD_RESULT const& result)
{
    if (result != FMOD_OK)
    {
		std::ostringstream ostr;
		ostr << "FMOD error ! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		throw std::exception(ostr.str().c_str());
    }
}

void SoundManager::initSystem()
{
	FMOD_RESULT result;
	unsigned int version;
	int numdrivers;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	char name[256];

	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&system);
	errCheck(result);

	result = system->getVersion(&version);
	errCheck(result);

	if (version < FMOD_VERSION)
	{
		std::ostringstream ostr;
		ostr << "Error! You are using an old version of FMOD " << version << ". This program requires " << FMOD_VERSION << std::endl;
		throw std::exception(ostr.str().c_str());
	}

	result = system->getNumDrivers(&numdrivers);
	errCheck(result);

	if (numdrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		errCheck(result);
	}
	else
	{
		result = system->getDriverCaps(0, &caps, 0, &speakermode);
		errCheck(result);

		/*
		Set the user selected speaker mode.
		*/
		result = system->setSpeakerMode(speakermode);
		errCheck(result);

		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			/*
			The user has the 'Acceleration' slider set to off! This is really bad
			for latency! You might want to warn the user about this.
			*/
			result = system->setDSPBufferSize(1024, 10);
			errCheck(result);
		}

		result = system->getDriverInfo(0, name, 256, 0);
		errCheck(result);

		if (strstr(name, "SigmaTel"))
		{
			/*
			Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			PCM floating point output seems to solve it.
			*/
			result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
			errCheck(result);
		}
	}

	result = system->init(100, FMOD_INIT_3D_RIGHTHANDED, 0);

	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		/*
		Ok, the speaker mode selected isn't supported by this soundcard. Switch it
		back to stereo...
		*/
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		errCheck(result);

		/*
		... and re-init.
		*/
		result = system->init(100, FMOD_INIT_3D_RIGHTHANDED, 0);
	}
	errCheck(result);
}

FMOD::System* SoundManager::getSystem()
{
	if (!system)
	{
		initSystem();
	}

	return system;
}

SoundManager::SoundManager()
	: backgroundSound(NULL)
{
}

SoundManager::~SoundManager()
{
	clearCache();

	if (system)
	{
		backgroundSound->release();
	}
}

void SoundManager::release()
{
	system->release();
	system = NULL;
}

void SoundManager::clearCache()
{
	if (system)
	{
		BOOST_FOREACH(sound_map::value_type const& entry, soundMap)
		{
			entry.second->release();
		}
	}

	soundMap.clear();
}

void SoundManager::update(glm::vec3 const& cameraPos, glm::vec3 const& cameraForward, glm::vec3 const& cameraUp)
{
	FMOD::System* system = getSystem();

	system->set3DListenerAttributes(0, reinterpret_cast<FMOD_VECTOR const*>(&cameraPos),
		NULL,
		reinterpret_cast<FMOD_VECTOR const*>(&cameraForward),
		reinterpret_cast<FMOD_VECTOR const*>(&cameraUp));
	system->update();
}

void SoundManager::playBackgroundSound(std::string const& filename)
{
	if (backgroundSound)
	{
		backgroundSound->release();
		backgroundSound = NULL;
	}

	if (filename.empty())
	{
		return;
	}

	FMOD::System* system = getSystem();

	FMOD_RESULT result = system->createSound(filename.c_str(), FMOD_LOOP_NORMAL, NULL, &backgroundSound);
	errCheck(result);

	result = system->playSound(FMOD_CHANNEL_FREE, backgroundSound, false, NULL);
	errCheck(result);
}

void SoundManager::playSound(std::string const& filename, glm::vec3 const& position, float minDistance)
{
	FMOD::System* system = getSystem();

	FMOD_RESULT result;
	if (soundMap.count(filename) == 0)
	{
		result = system->createSound(filename.c_str(), FMOD_3D, NULL, &soundMap[filename]);
		errCheck(result);
	}

	FMOD::Channel* channel;
	result = system->playSound(FMOD_CHANNEL_FREE, soundMap[filename], true, &channel);
	errCheck(result);

	result = channel->set3DAttributes(reinterpret_cast<FMOD_VECTOR const*>(&position), NULL);
	errCheck(result);

	result = channel->set3DMinMaxDistance(minDistance, 10000.f);
	errCheck(result);

	channel->setPaused(false);
}
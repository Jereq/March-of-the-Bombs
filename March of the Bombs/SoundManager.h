#pragma once

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include <glm/glm.hpp>

#include "MChannel.h"

class SoundManager
{
private:
	static FMOD::System* system;

	glm::mat3 invBaseMat;
	glm::vec3 listenerPos;

	typedef std::map<std::string, FMOD::Sound*> sound_map;
	sound_map soundMap;

	FMOD::Sound* backgroundSound;
	MChannel* backgroundchannel;

	static void errCheck(FMOD_RESULT const& result);
	static void initSystem();
	static FMOD::System* getSystem();

public:
	typedef boost::shared_ptr<SoundManager> ptr;

	SoundManager();
	~SoundManager();

	static void release();

	void clearCache();

	void update(glm::vec3 const& cameraPos, glm::vec3 const& cameraForward, glm::vec3 const& cameraUp);

	void playBackgroundSound(std::string const& filename);
	void playBackgroundSound(std::string const& filename, float volume);
	MChannel* playSound(std::string const& filename, glm::vec3 const& position, float minDistance);
	MChannel* playSound(std::string const& filename, glm::vec3 const& position, float minDistance, bool loop);

	glm::mat3 getInverseBaseMatrix();
	glm::vec3 getListenerPosition();
};
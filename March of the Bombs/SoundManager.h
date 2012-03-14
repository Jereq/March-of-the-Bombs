#pragma once

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include <glm/glm.hpp>

class SoundManager
{
private:
	static FMOD::System* system;

	static void errCheck(FMOD_RESULT const& result);
	static void initSystem();
	static FMOD::System* getSystem();

	typedef std::map<std::string, FMOD::Sound*> sound_map;
	sound_map soundMap;

	FMOD::Sound* backgroundSound;

public:
	typedef boost::shared_ptr<SoundManager> ptr;

	SoundManager();
	~SoundManager();

	static void release();

	void clearCache();

	void update(glm::vec3 const& cameraPos, glm::vec3 const& cameraForward, glm::vec3 const& cameraUp);

	void playBackgroundSound(std::string const& filename);
	void playSound(std::string const& filename, glm::vec3 const& position, float minDistance);
};
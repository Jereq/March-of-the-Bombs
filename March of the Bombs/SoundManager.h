#pragma once

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include <glm/glm.hpp>

class MChannel;

/*
 * SoundManager deals with anything FMOD, in order to encapsulate it.
 */
class SoundManager
{
public:
	// The different types of filters allowed
	enum class FilterType
	{
		NORMAL,	// None
		ECHO,
		DAMPENING
	};

private:
	static FMOD::System* system;

	glm::mat3 invBaseMat;
	glm::vec3 listenerPos;

	FilterType filterType;

	typedef std::map<std::string, FMOD::Sound*> sound_map;
	sound_map soundMap;

	FMOD::Sound* backgroundSound;
	MChannel* backgroundchannel;

	static void errCheck(FMOD_RESULT const& result);
	static void initSystem();
	static FMOD::System* getSystem();

public:
	/*
	 * Shared pointer to pass instances of SoundManager around
	 */
	typedef boost::shared_ptr<SoundManager> ptr;

	/*
	 * constructor.
	 */
	SoundManager();
	~SoundManager();

	/*
	 * Release the underlying static resources not local to a single SoundManager.
	 */
	static void release();

	/*
	 * Release all sounds currently cached.
	 */
	void clearCache();

	/*
	 * Update the position of the listener. Should be called once per frame.
	 */
	void update(glm::vec3 const& cameraPos, glm::vec3 const& cameraForward, glm::vec3 const& cameraUp);

	/*
	 * Load the target sound and play it in the background. The sound will keep starting over once it reaches the end.
	 * If a background sound is already playing, that sound will stop playing.
	 */
	void playBackgroundSound(std::string const& filename);
	/*
	 * Load the target sound and play it in the background. The sound will keep starting over once it reaches the end.
	 * If a background sound is already playing, that sound will stop playing.
	 */
	void playBackgroundSound(std::string const& filename, float volume);

	/*
	 * Creates a channel with the given sound, loading it if not cahced. The the sound will be a 3D sound.
	 * The returned channel must be deleted once the sound is no longer needed.
	 */
	MChannel* playSound(std::string const& filename, glm::vec3 const& position, float minDistance);
	/*
	 * Creates a channel with the given sound, loading it if not cahced. The the sound will be a 3D sound.
	 * If loop is true, the sound will start over once it reaches the end.
	 * The returned channel must be deleted once the sound is no longer needed.
	 */
	MChannel* playSound(std::string const& filename, glm::vec3 const& position, float minDistance, bool loop);

	/*
	 * Get the transformation needed to transform a position relative to
	 * the listener in world coordinates to listener local corrdinates.
	 */
	glm::mat3 getInverseBaseMatrix();
	/*
	 * Get the current position of the listener in world coordinates.
	 */
	glm::vec3 getListenerPosition();

	/*
	 * Set the filter type to be used for all currently playing channels.
	 */
	void setFilterType(FilterType type);
	/*
	 * Get the currently selected filter type.
	 */
	FilterType getFilterType() const;
};
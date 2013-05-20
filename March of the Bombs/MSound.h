#pragma once

#include <fmod/fmod.hpp>

#include <string>

class MSound
{
private:
	MSound(MSound const&);
	MSound & operator= (MSound const&);

	FMOD::Sound* sound;

	void errCheck(FMOD_RESULT const& result);

public:
	MSound(FMOD::System* system, std::string const& filename, bool is3D);
	~MSound();

};

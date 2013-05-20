#include "MSound.h"

#include <sstream>

#include <fmod/fmod_errors.h>

void MSound::errCheck(FMOD_RESULT const& result)
{
    if (result != FMOD_OK)
    {
		std::ostringstream ostr;
		ostr << "FMOD error ! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		throw std::exception(ostr.str().c_str());
    }
}

MSound::MSound(FMOD::System* system, std::string const& filename, bool is3D)
{
	FMOD_RESULT result;

	FMOD_MODE mode = FMOD_DEFAULT;
	if (is3D)
	{
		mode |= FMOD_3D;
	}

	result = system->createSound(filename.c_str(), FMOD_3D, nullptr, &sound);
	errCheck(result);
}

MSound::~MSound()
{
	errCheck(sound->release());
}
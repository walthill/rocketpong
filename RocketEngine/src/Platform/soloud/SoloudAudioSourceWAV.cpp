#include "SoloudAudioSourceWAV.h"
#include <RocketEngine/core/Log.h>

namespace RKTEngine
{
	SoloudAudioSourceWAV::SoloudAudioSourceWAV(const std::string& path, AudioFileType type) :
		AudioSource(type)
	{
		load(path);
	}

	SoloudAudioSourceWAV::~SoloudAudioSourceWAV()
	{

	}

	void SoloudAudioSourceWAV::load(const std::string& path)
	{
		//TODO: register load with asset manager??
		SoLoud::result result = mSource.load(path.c_str());

		if (result == SoLoud::SOLOUD_ERRORS::SO_NO_ERROR)
		{
			mInitialized = true;
			RKT_CORE_TRACE("SoloudAudioSourceWAV :: File loaded successfully. Path:'{0}'", path);
		}
		else if (result == SoLoud::SOLOUD_ERRORS::FILE_NOT_FOUND)
		{
			RKT_CORE_ERROR("SoloudAudioSourceWAV :: File not found. Path:'{0}'", path);
		}
	}
}
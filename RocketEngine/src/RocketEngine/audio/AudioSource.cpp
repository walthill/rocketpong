#include "AudioSource.h"
#include <RocketEngine/audio/AudioEngine.h>
#include  <Platform/soloud/SoloudAudioSourceWAV.h>

namespace RKTEngine
{
	AudioSource::AudioSource(AudioFileType type) :
		mFileType(type)
	{
	}

	AudioSource* AudioSource::create(const std::string& path, AudioFileType fileType)
	{
		if (AudioEngine::getAPI() == AudioEngine::API::SOLOUD)
		{
			switch (fileType)
			{
				case AudioSource::AudioFileType::NONE:	break;
				case AudioSource::AudioFileType::WAV:	return new SoloudAudioSourceWAV(path, fileType);	break;
				case AudioSource::AudioFileType::MP3:	break;
			}
		}
		return nullptr;
	}
}
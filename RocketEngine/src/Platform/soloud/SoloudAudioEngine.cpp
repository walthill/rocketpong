#include "SoloudAudioEngine.h"
#include "SoloudAudioSourceWAV.h"

namespace RKTEngine
{
	void SoloudAudioEngine::initialize()
	{
		mAudioEngine.init();
	}

	void SoloudAudioEngine::cleanup()
	{
		mAudioEngine.deinit();
	}


	int SoloudAudioEngine::play(AudioSource* source)
	{
		switch (source->getType())
		{
			case AudioSource::AudioFileType::WAV:	return playWAV(dynamic_cast<SoloudAudioSourceWAV*>(source));	break;
			case AudioSource::AudioFileType::MP3:	break;
		}

		return -1;
	}

	int SoloudAudioEngine::playWAV(SoloudAudioSourceWAV* source)
	{
		//TODO: turn this into a message event
		if (source != nullptr)
		{
			return mAudioEngine.play(*source->getWAV());
		}

		return -1;
	}

}
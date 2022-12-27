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


	uint32 SoloudAudioEngine::play(AudioSource* source, float vol, float pan, bool isLooping, bool paused)
	{
		switch (source->getType())
		{
			case AudioSource::AudioFileType::WAV:	return playWAV(dynamic_cast<SoloudAudioSourceWAV*>(source), vol, pan, isLooping, paused);	break;
			case AudioSource::AudioFileType::MP3:	break;
		}

		return -1;
	}

	void SoloudAudioEngine::stop(AudioSource* source)
	{
		switch (source->getType())
		{
			case AudioSource::AudioFileType::WAV:	stopWAV(dynamic_cast<SoloudAudioSourceWAV*>(source));	break;
			case AudioSource::AudioFileType::MP3:	break;
		}

	}

	uint32 SoloudAudioEngine::playWAV(SoloudAudioSourceWAV* source, float vol, float pan, bool isLooping, bool paused)
	{
		if (source != nullptr)
		{
			auto handle = mAudioEngine.play(*source->getWAV());
			mAudioEngine.setVolume(handle, vol);
			mAudioEngine.setLooping(handle, isLooping);
			mAudioEngine.setPan(handle, pan);            
			mAudioEngine.setPause(handle, paused);
			return handle;
		}

		return UINT32_MAX;
	}

	void SoloudAudioEngine::stopWAV(SoloudAudioSourceWAV* source)
	{
		if (source != nullptr)
		{
			//source->getWAV()->stop();
			mAudioEngine.stopAudioSource(*source->getWAV());
		}
	}

}
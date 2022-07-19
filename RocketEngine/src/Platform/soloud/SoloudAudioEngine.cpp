#include "SoloudAudioEngine.h"
#include "SoloudAudioSourceWAV.h"
#include <RocketEngine/core/EngineCore.h>
#include <RocketEngine/core/MessageManager.h>

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


	uint32 SoloudAudioEngine::play(AudioSource* source, float vol, float pan, bool paused)
	{
		switch (source->getType())
		{
			case AudioSource::AudioFileType::WAV:	return playWAV(dynamic_cast<SoloudAudioSourceWAV*>(source), vol, pan, paused);	break;
			case AudioSource::AudioFileType::MP3:	break;
		}

		return -1;
	}

	uint32 SoloudAudioEngine::playWAV(SoloudAudioSourceWAV* source, float vol, float pan, bool paused)
	{
		if (source != nullptr)
		{
			auto handle = mAudioEngine.play(*source->getWAV());
			mAudioEngine.setVolume(handle, vol);
			mAudioEngine.setPan(handle, pan);            
			mAudioEngine.setPause(handle, paused);
			return handle;
		}

		return UINT32_MAX;
	}

}
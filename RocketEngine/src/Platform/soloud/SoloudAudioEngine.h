#ifndef SOLOUD_AUDIO_ENG_H
#define SOLOUD_AUDIO_ENG_H

#include <soloud.h>
#include <RocketEngine/audio/AudioEngine.h>

namespace RKTEngine
{
	class SoloudAudioSourceWAV;

	class SoloudAudioEngine : public AudioEngine
	{
		public:
			virtual void initialize() override;
			virtual void cleanup() override;
			virtual uint32 play(AudioSource* source, float vol = -1.0f, float pan = 0.0f, bool paused = false) override;

		private:
			uint32 playWAV(SoloudAudioSourceWAV* source, float vol = -1.0f, float pan = 0.0f, bool paused = false);
			
			SoLoud::Soloud mAudioEngine;
	};
}

#endif // !SOLOUD_AUDIO_ENG_H

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
			virtual int play(AudioSource* source) override;

		private:
			int playWAV(SoloudAudioSourceWAV* source);
			
			SoLoud::Soloud mAudioEngine;
	};
}

#endif // !SOLOUD_AUDIO_ENG_H

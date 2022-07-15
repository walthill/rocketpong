#ifndef AUDIO_ENGINE_API_H
#define AUDIO_ENGINE_API_H

#include <RocketEngine/audio/AudioSource.h>

namespace RKTEngine
{
	class AudioEngine
	{
		public:
			enum class API { NONE = 0, SOLOUD = 1 };

			virtual void initialize() =0;
			virtual void cleanup() =0;
			virtual int play(AudioSource* source) =0;

			inline static AudioEngine::API getAPI() { return msAPI; };
		private:
			static AudioEngine::API msAPI;
	};
}

#endif // !AUDIO_ENGINE_API_H

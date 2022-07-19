#ifndef AUDIO_ENGINE_API_H
#define AUDIO_ENGINE_API_H

#include <RocketEngine/audio/AudioSource.h>
#include <RocketEngine/Defines.h>

namespace RKTEngine
{
	class AudioEngine
	{
		public:
			enum class API { NONE = 0, SOLOUD = 1 };

			virtual void initialize() =0;
			virtual void cleanup() =0;
			virtual uint32 play(AudioSource* source, float vol = -1.0f, float pan = 0.0f, bool paused = false) =0;

			inline static AudioEngine::API getAPI() { return msAPI; };
		private:
			static AudioEngine::API msAPI;
	};
}

#endif // !AUDIO_ENGINE_API_H

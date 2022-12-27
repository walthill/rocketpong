#ifndef AUDIO_COMMANDS_H
#define AUDIO_COMMANDS_H

#include "AudioEngine.h"

namespace RKTEngine
{
	class AudioCommand
	{
		public:
			inline static void initialize()
			{
				spAudioAPI->initialize();
			}

			inline static void cleanup()
			{
				spAudioAPI->cleanup();
			}


			inline static uint32 play(AudioSource* source, float vol = -1.0f, float pan = 0.0f, bool isLooping = false, bool paused = false)
			{
				return spAudioAPI->play(source, vol, pan, isLooping, paused);
			}

			inline static void stop(AudioSource* source)
			{
				spAudioAPI->stop(source);
			}

		private:
			static AudioEngine* spAudioAPI;
	};
}

#endif // !AUDIO_COMMANDS_H

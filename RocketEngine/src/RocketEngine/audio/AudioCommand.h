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


			inline static int play(AudioSource* source)
			{
				return spAudioAPI->play(source);
			}

		private:
			static AudioEngine* spAudioAPI;
	};
}

#endif // !AUDIO_COMMANDS_H

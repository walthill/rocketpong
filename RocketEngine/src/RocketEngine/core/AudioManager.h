#ifndef AUDIO_MAN_H
#define AUDIO_MAN_H

#include <RocketEngine/input/message/Message.h>
#include <RocketEngine/input/message/AudioPlayWAVMessage.h>
#include <RocketEngine/input/message/AudioStopWAVMessage.h>

namespace RKTEngine
{
	class AudioManager
	{
		public:
			AudioManager();
			~AudioManager();

			bool playSound(RKTEngine::AudioPlayWAVMessage& msg);
			bool stopSound(RKTEngine::AudioStopWAVMessage& msg);

			void onMessage(RKTEngine::Message& message);
			
		private:

	};
}

#endif // !AUDIO_MAN_H

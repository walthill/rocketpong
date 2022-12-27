#ifndef AUDIO_STOP_WAV_MSG_H
#define AUDIO_STOP_WAV_MSG_H

#include "Message.h"
#include <sstream>
#include <RocketEngine/audio/AudioSource.h>

namespace RKTEngine
{
	class AudioStopWAVMessage : public Message
	{
	public:
		AudioStopWAVMessage(AudioSource* audio, float vol = -1.0f, float pan = 0.0f, bool paused = false)
			: mpAudio(audio), Message(MessageType::AUDIO_PLAY_WAV) {};

		~AudioStopWAVMessage() {};

		inline AudioSource* getAudioSource() { return mpAudio; }
		
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "AudioPlayWAVMessage: Playing WAV";
			return ss.str();
		};

		EVENT_CLASS_TYPE(AUDIO_STOP_WAV)
		EVENT_CLASS_CATEGORY(MSG_CATEGORY_AUDIO)

	private:
		AudioSource* mpAudio;
	};
}
#endif // !AUDIO_PLAY_WAV_MSG_H

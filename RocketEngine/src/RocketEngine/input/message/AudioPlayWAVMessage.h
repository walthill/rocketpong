#ifndef AUDIO_PLAY_WAV_MSG_H
#define AUDIO_PLAY_WAV_MSG_H

#include "Message.h"
#include <sstream>
#include <RocketEngine/audio/AudioSource.h>

namespace RKTEngine
{
	class AudioPlayWAVMessage : public Message
	{
		public:
			AudioPlayWAVMessage(AudioSource* audio, float vol = -1.0f, float pan = 0.0f, bool isLooping = false, bool paused = false)
				: mpAudio(audio), mVolume(vol), mPan(pan), mLooping(isLooping), mPaused(paused), Message(MessageType::AUDIO_PLAY_WAV) {};

			~AudioPlayWAVMessage() {};

			inline AudioSource* getAudioSource() { return mpAudio; }
			inline float getVolume() { return mVolume; }
			inline float getPan() { return mPan; }
			inline bool isPaused() { return mPaused; }
			inline bool isLooping() { return mLooping; }

			std::string toString() const override
			{
				std::stringstream ss;
				ss << "AudioPlayWAVMessage: Playing WAV";
				return ss.str();
			};

			EVENT_CLASS_TYPE(AUDIO_PLAY_WAV)
			EVENT_CLASS_CATEGORY(MSG_CATEGORY_AUDIO)

		private:
			AudioSource* mpAudio;
			float mVolume = -1.0f;
			float mPan = 0.0f;
			bool mPaused = false;
			bool mLooping = false;
	};
}
#endif // !AUDIO_PLAY_WAV_MSG_H

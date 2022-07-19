#include "AudioManager.h"
#include <RocketEngine/audio/AudioCommand.h>

namespace RKTEngine
{
	AudioManager::AudioManager()
	{
		AudioCommand::initialize();

		
	}

	AudioManager::~AudioManager()
	{
		AudioCommand::cleanup();
	}

	void AudioManager::onMessage(RKTEngine::Message& message)
	{
		RKTEngine::MessageDispatcher dispatcher(message);
		dispatcher.dispatch<RKTEngine::AudioPlayWAVMessage>(RKT_BIND_MESSAGE_FN(AudioManager::playSound));
	}

	bool AudioManager::playSound(RKTEngine::AudioPlayWAVMessage& msg)
	{
		AudioCommand::play(msg.getAudioSource(), msg.getVolume(), msg.getPan(), msg.isPaused());
		return true;
	}
}
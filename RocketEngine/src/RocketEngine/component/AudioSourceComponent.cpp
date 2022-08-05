#include "AudioSourceComponent.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/audio/AudioSource.h"
#include <RocketEngine/core/MessageManager.h>
#include <RocketEngine/core/Log.h>
#include <RocketEngine/input/message/AudioPlayWAVMessage.h>

namespace RKTEngine
{
	AudioSourceComponent::AudioSourceComponent(const ComponentId& id) :
		Component(id)
	{
	}

	AudioSourceComponent::~AudioSourceComponent()
	{
	}
	
	void AudioSourceComponent::load()
	{
		if (!mAudioSourceData.audioFileName.empty())
		{
			mAudioSourceData.audioSource = EngineCore::getInstance()->getAssetManager()->loadAudioSource(mAudioSourceData.audioFileName);
		}
		else
		{
			RKT_CORE_ERROR("Audio name is empty!");
		}
	}

	void AudioSourceComponent::cleanup()
	{

	}

	void AudioSourceComponent::play(float vol)
	{
		if (!mAudioSourceData.audioSource->isInitialized())
		{
			RKT_WARN("AudioSourceComponent::Attempting to play a missing audio source!");
			return;
		}

		auto pMessage = new AudioPlayWAVMessage(mAudioSourceData.audioSource, vol);
		EngineCore::getInstance()->getMessageManager()->addMessage(pMessage, 1);
	}

	void AudioSourceComponent::setData(const AudioSourceComponentData& data)
	{
		mAudioSourceData = data;
	}
}
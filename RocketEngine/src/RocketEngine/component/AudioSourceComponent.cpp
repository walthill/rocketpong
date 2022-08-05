#include "AudioSourceComponent.h"
#include "RocketEngine/core/EngineCore.h"
#include <RocketEngine/core/MessageManager.h>
#include "RocketEngine/asset/AssetManager.h"
#include <RocketEngine/core/Log.h>
#include "RocketEngine/audio/AudioSource.h"
#include <RocketEngine/input/message/AudioPlayWAVMessage.h>

namespace RKTEngine
{
	const float AudioSourceComponent::mDEFAULT_VOL = -1.0f;
	const float AudioSourceComponent::mDEFAULT_PAN = 0.0f;


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

	void AudioSourceComponent::play(float vol, float pan)
	{
		if (!mAudioSourceData.audioSource->isInitialized())
		{
			RKT_WARN("AudioSourceComponent::Attempting to play a missing audio source!");
			return;
		}

		setVolume(vol);
		setPan(pan);
		setPauseFlag(false);
		auto pMessage = new AudioPlayWAVMessage(mAudioSourceData.audioSource, mAudioSourceData.mVolume, mAudioSourceData.mPan, mAudioSourceData.mPaused);
		EngineCore::getInstance()->getMessageManager()->addMessage(pMessage, 1);
	}

	void AudioSourceComponent::setVolume(float vol)
	{
		mAudioSourceData.mVolume = vol;
	}

	void AudioSourceComponent::setPan(float pan)
	{
		mAudioSourceData.mPan = pan;
	}

	void AudioSourceComponent::setPauseFlag(bool pause)
	{
		mAudioSourceData.mPaused = pause;
	}

	void AudioSourceComponent::setData(const AudioSourceComponentData& data)
	{
		mAudioSourceData = data;
	}
}
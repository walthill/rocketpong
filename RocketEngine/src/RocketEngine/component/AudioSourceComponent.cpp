#include "AudioSourceComponent.h"
#include "RocketEngine/core/EngineCore.h"
#include <RocketEngine/core/MessageManager.h>
#include "RocketEngine/asset/AssetManager.h"
#include <RocketEngine/core/Log.h>
#include "RocketEngine/audio/AudioSource.h"
#include <RocketEngine/input/message/AudioPlayWAVMessage.h>
#include <RocketEngine/input/message/AudioStopWAVMessage.h>

namespace RKTEngine
{
	const float AudioSourceComponent::sDEFAULT_VOL = -1.0f;
	const float AudioSourceComponent::sDEFAULT_PAN = 0.0f;


	AudioSourceComponent::AudioSourceComponent(const ComponentId& id) :
		Component(id)
	{
	}

	AudioSourceComponent::~AudioSourceComponent()
	{
	}
	
	void AudioSourceComponent::load()
	{
		if (!mAudioSourceData.mAudioFileName.empty())
		{
			mAudioSourceData.audioSource = EngineCore::getInstance()->getAssetManager()->loadAudioSource(mAudioSourceData.mAudioFileName);
		}
		else
		{
			RKT_CORE_ERROR("Audio name is empty!");
		}
	}

	void AudioSourceComponent::cleanup()
	{

	}

	void AudioSourceComponent::play(float vol, float pan, bool isLooping)
	{
		if (mAudioSourceData.audioSource == nullptr)
			return;

		if (!mAudioSourceData.audioSource->isInitialized())
		{
			RKT_WARN("AudioSourceComponent::Attempting to play a missing audio source!");
			return;
		}

		if (!mAudioSourceData.isEnabled)
			return;

		setVolume(vol);
		setPan(pan);
		setLooping(isLooping);
		setPauseFlag(false);
		auto pMessage = new AudioPlayWAVMessage(mAudioSourceData.audioSource, mAudioSourceData.mVolume, mAudioSourceData.mPan, mAudioSourceData.mLooping, mAudioSourceData.mPaused);
		EngineCore::getInstance()->getMessageManager()->addMessage(pMessage, 1);
	}

	void AudioSourceComponent::stop()
	{
		if (mAudioSourceData.audioSource == nullptr)
			return;

		if (!mAudioSourceData.audioSource->isInitialized())
		{
			RKT_WARN("AudioSourceComponent::Attempting to play a missing audio source!");
			return;
		}

		if (!mAudioSourceData.isEnabled)
			return;

		auto pMessage = new AudioStopWAVMessage(mAudioSourceData.audioSource);
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

	void AudioSourceComponent::setLooping(bool isLooping)
	{
		mAudioSourceData.mLooping = isLooping;
	}

	void AudioSourceComponent::setData(const AudioSourceComponentData& data)
	{
		mAudioSourceData = data;
	}
}
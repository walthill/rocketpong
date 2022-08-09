#ifndef AUDIO_SRC_COMP_H
#define AUDIO_SRC_COMP_H

#include "Component.h"

namespace RKTEngine
{

	class AudioSource;

	/***************************************************************************//**
	 * @brief 	Data used in every audio source component.
	 *
	 * This struct contains data and constructors that will serve as foundational data
	 * for every audio source
	 ******************************************************************************/
	struct AudioSourceComponentData
	{
		std::string mAudioFileName;
		AudioSource* audioSource;
		float mVolume = -1.0f;
		float mPan = 0.0f;
		bool mPaused = false;

		///Default constructor sets all values to zero
		AudioSourceComponentData() : mAudioFileName(""), audioSource(nullptr) {};
		AudioSourceComponentData(const std::string& audioFile, float vol = -1.0f, float pan = 0.0f, bool pause = false) 
			: mAudioFileName(audioFile), audioSource(nullptr), mVolume(vol), mPan(pan), mPaused(pause) {};

		template<class Archive>
		void save(Archive& archive) const
		{
			if (this != nullptr)
				archive(CEREAL_NVP(mAudioFileName), CEREAL_NVP(mVolume), CEREAL_NVP(mPan));
		}

		template<class Archive>
		void load(Archive& archive)
		{
			if (!mAudioFileName.empty())
				archive(CEREAL_NVP(mAudioFileName), CEREAL_NVP(mVolume), CEREAL_NVP(mPan));
		}
	};

	const AudioSourceComponentData ZERO_AUDIO_SRC_DATA;

	/***************************************************************************//**
	 * @brief 	The component for Audio Sources.
	 *
	 * This class, derived from Component, contains data needed to play sounds.
	 ******************************************************************************/
	class AudioSourceComponent : public Component
	{
	public:

		/**********************************************************************//**
		* Creates component with the given id value.
		*
		* @param id ComponentId value assigned to this instance of the Component class
		*************************************************************************/
		AudioSourceComponent(const ComponentId& id);

		///Default deconstructor
		~AudioSourceComponent();

		///Load audio from file
		void load();
		///Cleanup
		void cleanup();

		void play(float vol = mDEFAULT_VOL, float pan = mDEFAULT_PAN);
		void setVolume(float vol);
		void setPan(float pan);
		void setPauseFlag(bool pause);

		///Access the Audio Source component data
		AudioSourceComponentData* getData() { return &mAudioSourceData; }
	
		void setData(const AudioSourceComponentData& data);

	private:
		static const float mDEFAULT_VOL;
		static const float mDEFAULT_PAN;
		AudioSourceComponentData mAudioSourceData;
	};
}
#endif // !MESH_COMP_H

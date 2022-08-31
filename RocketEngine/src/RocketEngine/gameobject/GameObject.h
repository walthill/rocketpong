/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
		 GameObject.h
	=========================
	This file contains the definition for the GameObject class.
	The class handles calculations for GameObjects and holds id's 
	for components assigned to the GameObject.

********/

#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include <RKTUtils/Trackable.h>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include "RocketEngine/Defines.h"
#include <RocketEngine/component/ComponentDefines.h>
#include <RocketEngine/actor/Actor.h>

namespace RKTEngine
{
	class NativeScriptComponent;

	/***************************************************************************//**
	 * @brief 	GameObject class with identifiers for components.
	 *
	 * This class contains a GameObject's component identifiers and controls the
	 * processing of transform data.
	 ******************************************************************************/
	class GameObject : public RKTUtil::Trackable
	{
		friend class Actor;
		friend class GameObjectManager;

		public:
			///Default constructor
			GameObject();

			GameObject(GameObjectId id);

			///Default deconstructor
			~GameObject();

			///Cleanup
			void cleanup();

			/**********************************************************************//**
			* Process transform calulations and send data to Mesh Component.
			*
			* @param elapsedTime Frame timing
			*************************************************************************/
			void update(float elapsedTime);

			///Access the GameObject's id
			inline uint32 getId() { return mId; }
			///Access the GameObject's transform component id
			inline ComponentId getTransformId() { return mTransformId; };
			///Access the GameObject's mesh component id
			inline ComponentId getSpriteId() { return mSpriteId; };
			///Access the GameObject's text component id
			inline ComponentId getLabelId() { return mLabelId; };
			inline ComponentId getColliderId() { return mColliderId; };
			inline ComponentId getAudioSourceId() { return mAudioSourceId; };
			inline ComponentId getButtonId() { return mButtonId; };
			inline ComponentId getNativeScriptId() { return mNativeScriptId; };

			///Acesss the GameObject's transform 
			TransformComponent* getTransform() { return mpTransform; }
			///Acesss the GameObject's sprite 
			SpriteComponent* getSprite();
			///Acesss the GameObject's box collider
			BoxColliderComponent* getBoxCollider();
			///Acesss the GameObject's ui label
			TextComponent* getUILabel();
			ButtonComponent* getButton();
			AudioSourceComponent* getAudioSource();
			NativeScriptComponent* getScript();

			std::string name;

		private:
			uint32 mId;
			ComponentId mSpriteId;
			ComponentId mTransformId;
			ComponentId mLabelId;
			ComponentId mButtonId;
			ComponentId mColliderId;
			ComponentId mAudioSourceId;
			ComponentId mNativeScriptId;

			TransformComponent* mpTransform = nullptr;

			TransformComponent getTransform_Serialize() const { return *mpTransform; }
			SpriteComponent* getSprite_Serialize() const;
			BoxColliderComponent* getBoxCollider_Serialize() const;
			AudioSourceComponent* getAudioSource_Serialize() const;
			TextComponent* getUILabel_Serialize() const;
			ButtonComponent* getButton_Serialize() const;
			NativeScriptComponent* getScript_Serialize() const;

			void addSpriteComponent(const SpriteComponentData& data);
			void addBoxColliderComponent(const BoxColliderData& data);
			void addAudioSourceComponent(const AudioSourceComponentData& data);
			void addUILabelComponent(const TextData& data);
			void addButtonComponent(const ButtonComponentData& data);
			void addNativeScriptComponent();

			void setName();

			/**********************************************************************//**
			* Set GameObject's unique id
			*
			* @param id GameObject identifier
			*************************************************************************/
			void setId(uint32 id) { mId = id; }

			/**********************************************************************//**
			* Set GameObject's transform reference
			*
			* @param comp TransformComponent reference
			*************************************************************************/
			void setTransformHandle(TransformComponent* comp) { mpTransform = comp; }

			/**********************************************************************//**
			* Set GameObject's transform id
			*
			* @param transfromId Component identifier
			*************************************************************************/
			void connectTransform(ComponentId transfromId) { mTransformId = transfromId; }

			/**********************************************************************//**
			* Set GameObject's sprite id
			*
			* @param spriteId SpriteComponent identifier
			*************************************************************************/
			void connectSprite(ComponentId spriteId) { mSpriteId = spriteId; }

			/**********************************************************************//**
			* Set GameObject's label id
			*
			* @param labelId TextComponent identifier
			*************************************************************************/
			void connectLabel(ComponentId labelId) { mLabelId = labelId; };

			/**********************************************************************//**
			* Set GameObject's button id
			*
			* @param labelId TextComponent identifier
			*************************************************************************/
			void connectButton(ComponentId buttonId) { mButtonId = buttonId; }

			/**********************************************************************//**
			* Set GameObject's collider id
			*
			* @param labelId TextComponent identifier
			*************************************************************************/
			void connectCollider(ComponentId colliderId) { mColliderId = colliderId; }

			/**********************************************************************//**
			* Set GameObject's audio source id
			*
			* @param labelId TextComponent identifier
			*************************************************************************/
			void connectAudioSource(ComponentId audioSrcId) { mAudioSourceId = audioSrcId; }

			/**********************************************************************//**
			* Set GameObject's script id
			*
			* @param labelId TextComponent identifier
			*************************************************************************/
			void connectNativeScript(ComponentId scriptId) { mNativeScriptId = scriptId; }

			void onDeserialize(const TransformData& transformComponent);

			friend cereal::access;

			template<class Archive>
			void save(Archive& archive) const
			{
				auto tr = getTransform_Serialize();
				auto spr = getSprite_Serialize();
				auto box = getBoxCollider_Serialize();
				auto aud = getAudioSource_Serialize();
				auto lbl = getUILabel_Serialize();
				auto btn = getButton_Serialize();
				auto pActor = getScript_Serialize() == nullptr ? std::shared_ptr<Actor>(nullptr) : std::shared_ptr<Actor>(getScript_Serialize()->pInstance);
				
				auto sprData = spr == nullptr ? nullptr : spr->getData();
				auto boxData = box == nullptr ? nullptr : box->getData();
				auto audData = aud == nullptr ? nullptr : aud->getData();
				auto lblData = lbl == nullptr ? nullptr : lbl->getData();
				auto btnData = btn == nullptr ? nullptr : btn->getData();

				archive(CEREAL_NVP(name), CEREAL_NVP(mId), cereal::make_nvp("TransformComponent", tr.getData()), cereal::make_nvp("SpriteComponent", *sprData),
						cereal::make_nvp("BoxColliderComponent", *boxData), cereal::make_nvp("AudioSourceComponent", *audData),
						cereal::make_nvp("UILabelComponent", *lblData), MAKE_NVP("ButtonComponent", *btnData), MAKE_NVP("Native Script", pActor));
			}

			template<class Archive>
			void load(Archive& archive)
			{
				auto tr = ZERO_TRANSFORM_DATA;
				auto spr = ZERO_SPRITE_DATA;
				auto box = ZERO_BOX_COLLIDER_DATA;
				auto aud = ZERO_AUDIO_SRC_DATA;
				auto lbl = ZERO_LABEL_DATA;
				auto btn = ZERO_BTN_DATA;
				std::shared_ptr<Actor> pActor;

				archive(CEREAL_NVP(name), CEREAL_NVP(mId), cereal::make_nvp("TransformComponent", tr), cereal::make_nvp("SpriteComponent", spr),
						cereal::make_nvp("BoxColliderComponent", box), cereal::make_nvp("AudioSourceComponent", aud), cereal::make_nvp("UILabelComponent", lbl),
						MAKE_NVP("ButtonComponent", btn), MAKE_NVP("Native Script", pActor));

				onDeserialize(tr);

				if (!spr.mSpriteName.empty())
					addSpriteComponent(spr);
				if (!box.tag.empty())
				{
					addBoxColliderComponent(box);
				}
				if (!aud.mAudioFileName.empty())
					addAudioSourceComponent(aud);
				if (!lbl.mFontName.empty())
					addUILabelComponent(lbl);
				if (!btn.mSprite.mSpriteName.empty() && !btn.mHighlightSprite.mSpriteName.empty()
					&& !btn.mText.mFontName.empty())
				{
					addButtonComponent(btn);
				}
				if (pActor.get() != nullptr)
				{					
					addNativeScriptComponent();
					pActor->onDeserialize(mId);
				}
			}
	};
}
#endif // !GAME_OBJ_H

#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

#include "Component.h"
#include "SpriteComponent.h"
#include "TextComponent.h"
#include "RocketEngine/render/buffers/Texture.h"
#include <functional>

namespace RKTEngine
{

	typedef std::function<void()> ButtonCallbackVoid;
	typedef std::function<void(bool)> ButtonCallbackBool;
	typedef std::function<void(int)> ButtonCallbackInt;

	struct ButtonComponentData
	{
		SpriteComponentData mSprite;
		SpriteComponentData mHighlightSprite;
		TextData mText;
		bool mHighlighted;
		ButtonCallbackVoid mCallbackVoid;
		ButtonCallbackBool mCallbackBool;	//wip
		ButtonCallbackInt mCallbackInt;		//wip

		ButtonComponentData() 
		{
			mHighlighted = false;
			mSprite = SpriteComponentData();
			mHighlightSprite = SpriteComponentData();
			mText = TextData();
			mCallbackVoid = nullptr;
			mCallbackBool = nullptr;
			mCallbackInt = nullptr;
		}

		ButtonComponentData(const SpriteComponentData& spr, const SpriteComponentData& highlightSpr, const TextData& tData)
			: mHighlighted(false)
		{
			mSprite = spr;
			mHighlightSprite = highlightSpr;
			mText = tData;
			mCallbackVoid = nullptr;
			mCallbackBool = nullptr;
			mCallbackInt = nullptr;
		}

		template<class Archive>
		void save(Archive& archive) const
		{
			if (this != nullptr)
				archive(CEREAL_NVP(mSprite), CEREAL_NVP(mHighlightSprite), CEREAL_NVP(mText));
		}

		template<class Archive>
		void load(Archive& archive)
		{
			try
			{
				archive(CEREAL_NVP(mSprite), CEREAL_NVP(mHighlightSprite), CEREAL_NVP(mText));
			}
			catch (cereal::Exception&)
			{
				archive.setNextName(nullptr);
				// Loading a key that doesn't exist results in an exception
				// Since "Not Found" is a valid condition for us, we swallow
				// this exception and the archive will not load anything
			}
		}
	};

	const ButtonComponentData ZERO_BTN_DATA;

	class ButtonComponent : public Component
	{
		friend class SceneManager;
	public:
		ButtonComponent(const ComponentId& id);
		~ButtonComponent();

		void load();
		void cleanup();

		const std::string& getText();

		///Access the mesh component data
		ButtonComponentData* getData() { return &mData; }

		void setText(const std::string& text);
		void setText(int text);
		void setData(const ButtonComponentData& data);
		
		void processSprite(const glm::vec2& position, const glm::vec2& scale, float rotationAngle);
		void processText(const glm::vec2& position, const glm::vec2& scale, float rotationAngle);
		void renderSprite();
		void renderText();
		
		void onSelected();
		void setOnSelected(ButtonCallbackVoid func);
		//void setOnSelected(ButtonCallbackBool func);
		//void setOnSelected(ButtonCallbackInt func);	//wip

	private:
		const std::string mTEXT_COLOR_UNIFORM = "textColor";
		const std::string mSHADER_ID = "text";
		const std::string mMODEL_UNIFORM = "model";

		ButtonComponentData mData;
		AtlasCoordinateData mAtlasCoords, mHighlightAtlasCoords;
		bool mAtlasInitialized = false, mHighlightAtlasInitialized = false;
		SpriteRenderData mRenderInfo;
		glm::mat4 mModelMatrix;

		AtlasCoordinateData calculateAtlasCoords(SpriteComponentData& sprData, int x, int y);
		void loadSprite(SpriteComponentData& sprData, bool isHighlightSpr = false);
		void loadText();
		void processSprite(SpriteComponentData& sprData, const glm::vec2& position, const glm::vec2& scale, float rotationAngle);
		void setHighlighted(bool selected);
	};
}

#endif // !BUTTON_COMPONENT_H

#ifndef UI_MAN_H
#define UI_MAN_H

#include <RocketEngine.h>

class UIManager : public RKTEngine::Actor
{
	public:
		UIManager();
		~UIManager();

		void onStart() override;
		void onDestroy() override;

		//GameObject handles
		unsigned int playButtonId;
		unsigned int exitButtonId;
		unsigned int controlsButtonId;
		unsigned int controlsBackButtonId;

	private:
		void onQuitSelected();
		void onStartSelected();
		void onControlsSelected();
		void onControlsBackSelected();

		//Save & load Actor variables here.
		#pragma region Serialization

		friend cereal::access;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(playButtonId), CEREAL_NVP(exitButtonId), CEREAL_NVP(controlsButtonId), CEREAL_NVP(controlsBackButtonId));
		}
		
		virtual void onDeserialize(int id) override;

		#pragma endregion
};


#endif // !UI_MAN_H

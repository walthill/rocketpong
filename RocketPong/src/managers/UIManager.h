#ifndef UI_MAN_H
#define UI_MAN_H

#include <RocketEngine.h>

class UIManager : public RKTEngine::Actor
{
	public:
		UIManager();
		~UIManager();

		void onStart() override;

		void onQuitSelected();
		void onStartSelected();

		//GameObject handles
		unsigned int playButtonId;
		unsigned int exitButtonId;
	
	private:

		//Save & load Actor variables here.
		#pragma region Serialization

		friend cereal::access;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(playButtonId), CEREAL_NVP(exitButtonId));
		}

protected:
	virtual void onDeserialize(int id) override;

		#pragma endregion
};

REGISTER_ACTOR(UIManager);

#endif // !UI_MAN_H

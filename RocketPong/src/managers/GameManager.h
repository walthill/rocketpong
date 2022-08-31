#ifndef GAME_MAN_H
#define GAME_MAN_H

#include <RocketEngine/actor/Actor.h>

class GameManager : public RKTEngine::Actor
{
	public:
		GameManager();
		~GameManager();

		void onCreate() override;
		void onUpdate() override;
		
		void score(bool p1Scored);
		void reset();

		void setBallHandle(int ballId);
		void setScoreUI(int p1ScoreId, int p2ScoreId);

		int mP1Score, mP2Score;
	private:
		unsigned int mBallId;
		unsigned int mP1ScoreId;
		unsigned int mP2ScoreId;

		
	//Save & load Actor variables here.
	#pragma region Serialization

		friend cereal::access;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(mBallId), CEREAL_NVP(mP1ScoreId), CEREAL_NVP(mP2ScoreId));
		}

protected:
	virtual void onDeserialize(int id) override;

	#pragma endregion
};

REGISTER_ACTOR(GameManager);

#endif // !GAME_MAN_H

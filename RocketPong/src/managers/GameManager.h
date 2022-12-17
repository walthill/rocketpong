#ifndef GAME_MAN_H
#define GAME_MAN_H

#include <RocketEngine/actor/Actor.h>
#include <RocketEngine.h>

class GameManager : public RKTEngine::Actor
{
	public:
		GameManager();
		~GameManager();

		void onStart() override;
		void onCreate() override;
		void onUpdate() override;
		
		void score(bool p1Scored);
		void reset();

		void setBallHandle(int ballId);
		void setScoreUI(int p1ScoreId, int p2ScoreId);
		bool isGameOver();
		void onPlaySelected();

		int mP1Score, mP2Score;
		
		//handles
		unsigned int pauseTextId;
		unsigned int resumeButtonId;
		unsigned int quitButtonId;

	private:
		unsigned int mBallId;
		unsigned int mP1ScoreId;
		unsigned int mP2ScoreId;
		const	 int mSCORE_TO_WIN = 11;

		void onQuitSelected();
		
	//Save & load Actor variables here.
	#pragma region Serialization

		friend cereal::access;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(mBallId), CEREAL_NVP(mP1ScoreId), CEREAL_NVP(mP2ScoreId), CEREAL_ACTOR(), 
				CEREAL_NVP(quitButtonId), CEREAL_NVP(resumeButtonId), CEREAL_NVP(pauseTextId));
		}
		
		void onDeserialize(int id) override;

	#pragma endregion

};


#endif // !GAME_MAN_H

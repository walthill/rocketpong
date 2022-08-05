#ifndef GAME_MAN_H
#define GAME_MAN_H

class GameManager
{
	public:
		GameManager();
		~GameManager();
		
		void score(bool p1Scored);
		void reset();

		void setScoreUI(int p1ScoreId, int p2ScoreId);

		int mP1Score, mP2Score;
	private:
		unsigned int mP1ScoreId;
		unsigned int mP2ScoreId;
};

#endif // !GAME_MAN_H

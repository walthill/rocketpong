#ifndef GAME_MAN_H
#define GAME_MAN_H

class GameManager
{
	public:
		GameManager();
		~GameManager();
		void score(bool p1Scored);
		void reset();

		int mP1Score, mP2Score;
	private:
};

#endif // !GAME_MAN_H

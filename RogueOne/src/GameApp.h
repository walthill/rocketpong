#ifndef GAME_APP_H
#define GAME_APP_H

#include <RKTUtils/Singleton.h>
#include <RKTUtils/PerformanceTracker.h>
#include <RKTUtils/Timer.h>
#include <RocketEngine/input/MessageDefines.h>

class Player;

class GameApp : public RKTUtil::Singleton<GameApp>
{
	friend class RKTUtil::Singleton<GameApp>;

	public:
		void clean();
		bool initialize();

		bool runGame();

		void update();
		void render();

		void onMessage(RKTEngine::Message& message);

		double getTime(); 

		bool quit(RKTEngine::ExitMessage& msg);

	protected:
		GameApp() {};
		~GameApp();
		
	private:
		const std::string mINIT_TRACKER_NAME = "init";
		const std::string mDRAW_TRACKER_NAME = "draw";
		const std::string mLOOP_TRACKER_NAME = "loop";
		int mFPS = 0;

		bool mIsRunning = false;
		bool mDisplayFrameTime = false;
		bool mShowFPS = false;

		//Performance tracker data
		RKTUtil::PerformanceTracker* pinitPerformanceTracker = nullptr;
		RKTUtil::PerformanceTracker* mpPerformanceTracker = nullptr;
		RKTUtil::Timer* mpFrameTimer = nullptr;

		static Player* spPlayer;

		void beginInit();
		void endInit();

};

#endif // !GAME_APP_H

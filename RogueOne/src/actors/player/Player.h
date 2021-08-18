#ifndef PLAYER_H
#define PLAYER_H

#include <glm/vec2.hpp>
#include "../Actor.h"

class Player : public Actor
{
	public:
		Player(const std::string& spriteToLoad, glm::vec2 position = glm::vec2(0, 0),
			glm::vec2 scale = glm::vec2(1, 1), float rotation = 0);
		~Player();

		virtual void onMessage(RKTEngine::Message& message) override;

	private:
		bool onKeyDown(RKTEngine::KeyDownMessage& message);
		bool onKeyUp(RKTEngine::KeyDownMessage& message);
};

#endif // !PLAYER_H

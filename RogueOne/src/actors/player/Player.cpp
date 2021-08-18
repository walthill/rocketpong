#include "Player.h"
#include "RocketEngine.h"

Player::Player(const std::string& spriteToLoad, glm::vec2 position, glm::vec2 scale, float rotation) :
	Actor()
{
	mpGameObject = RocketEngine->getEntityManager()->createPlayer(spriteToLoad, position, scale, rotation);
}

Player::~Player()
{
}

void Player::onMessage(RKTEngine::Message& message)
{
	Actor::onMessage(message);

	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::KeyDownMessage>(RKT_BIND_MESSAGE_FN(Player::onKeyDown));
}

#pragma region Callbacks

bool Player::onKeyDown(RKTEngine::KeyDownMessage& message)
{
	int keyCode = message.getKeyCode();

	if (keyCode == KeyCode::W)
	{
		auto transform = mpGameObject->getTransform();
		auto pos = transform->getPosition();
		transform->setPosition(pos.x, pos.y - 5);
	}
	else if (keyCode == KeyCode::A)
	{
		auto transform = mpGameObject->getTransform();
		auto pos = transform->getPosition();
		transform->setPosition(pos.x - 5, pos.y);
	}
	else if (keyCode == KeyCode::S)
	{
		auto transform = mpGameObject->getTransform();
		auto pos = transform->getPosition();
		transform->setPosition(pos.x, pos.y + 5);
	}
	else if(keyCode == KeyCode::D)
	{
		auto transform = mpGameObject->getTransform();
		auto pos = transform->getPosition();
		transform->setPosition(pos.x + 5, pos.y);
	}

	return true;
}

bool Player::onKeyUp(RKTEngine::KeyDownMessage& message)
{
	return true;
}

#pragma endregion

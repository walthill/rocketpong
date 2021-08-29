#include "Player.h"
#include "RocketEngine.h"
#include "../../GameApp.h"
#include "../../map/Map.h"

Player::Player(const std::string& texture, const std::string& tileName, int width, int height, glm::vec2 position, glm::vec2 scale, float rotation) :
	Actor()
{
	mpGameObject = RocketEngine->getEntityManager()->createPlayer(texture, tileName, width, height, position, scale, rotation);
}

Player::~Player()
{
}

bool Player::update(RKTEngine::UpdateMessage& message)
{
	return Actor::update(message);
}

void Player::onMessage(RKTEngine::Message& message)
{
	Actor::onMessage(message);

	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::UpdateMessage>(RKT_BIND_MESSAGE_FN(Player::update));
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
		transform->setPosition(pos.x, pos.y - Map::sTileSize);

		if (!GameApp::getInstance()->getMap()->isValidPosition(transform->getPosition()))
		{
			transform->setPosition(pos);
		}
	}
	else if (keyCode == KeyCode::A)
	{
		auto transform = mpGameObject->getTransform();
		auto pos = transform->getPosition();
		transform->setPosition(pos.x - Map::sTileSize, pos.y);
		if (!GameApp::getInstance()->getMap()->isValidPosition(transform->getPosition()))
		{
			transform->setPosition(pos);
		}
	}
	else if (keyCode == KeyCode::S)
	{
		auto transform = mpGameObject->getTransform();
		auto pos = transform->getPosition();
		transform->setPosition(pos.x, pos.y + Map::sTileSize);
		if (!GameApp::getInstance()->getMap()->isValidPosition(transform->getPosition()))
		{
			transform->setPosition(pos);
		}
	}
	else if(keyCode == KeyCode::D)
	{
		auto transform = mpGameObject->getTransform();
		auto pos = transform->getPosition();
		transform->setPosition(pos.x + Map::sTileSize, pos.y);
		if (!GameApp::getInstance()->getMap()->isValidPosition(transform->getPosition()))
		{
			transform->setPosition(pos);
		}
	}

	return true;
}

bool Player::onKeyUp(RKTEngine::KeyDownMessage& message)
{
	return true;
}

#pragma endregion

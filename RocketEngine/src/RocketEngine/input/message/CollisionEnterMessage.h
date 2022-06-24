#ifndef COLLISION_MSG_H
#define COLLISION_MSG_H

#include "Message.h"
#include <sstream>

namespace RKTEngine
{
	class BoxColliderComponent;

	class CollisionEnterMessage : public Message
	{
	public:
		CollisionEnterMessage(ComponentId colliderId, BoxColliderComponent* other)
			: colliderID(colliderId), otherCollider(other), Message(MessageType::COLLISION) {};

		~CollisionEnterMessage() { otherCollider = nullptr; };

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "CollisionEnterMessage: Shutting down";
			return ss.str();
		};

		ComponentId colliderID;
		BoxColliderComponent* otherCollider;

		EVENT_CLASS_TYPE(COLLISION)
			EVENT_CLASS_CATEGORY(MSG_CATEGORY_APP)
	};
}
#endif // !EXIT_MSG_H

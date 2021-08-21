#ifndef UPDATE_MSG_H
#define UPDATE_MSG_H

#include "Message.h"
#include <sstream>

namespace RKTEngine
{
	class UpdateMessage : public Message
	{
		public:
			UpdateMessage()
				: Message(MessageType::UPDATE) {};

			~UpdateMessage() {};

			std::string toString() const override
			{
				std::stringstream ss;
				ss << "UpdateMessage: Updating actors";
				return ss.str();
			};

			EVENT_CLASS_TYPE(UPDATE)
			EVENT_CLASS_CATEGORY(MSG_CATEGORY_APP)
	};
}
#endif // !Update_MSG_H

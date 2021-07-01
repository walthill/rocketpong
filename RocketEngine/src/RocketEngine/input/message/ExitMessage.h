#ifndef EXIT_MSG_H
#define EXIT_MSG_H

#include "Message.h"
#include <sstream>

namespace RKTEngine
{
	class ExitMessage : public Message
	{
		public:
			ExitMessage()
				: Message(MessageType::EXIT) {};

			~ExitMessage() {};

			std::string toString() const override
			{
				std::stringstream ss;
				ss << "ExitMessage: Shutting down";
				return ss.str();
			};

			EVENT_CLASS_TYPE(EXIT)
			EVENT_CLASS_CATEGORY(MSG_CATEGORY_APP)
		};
}
#endif // !EXIT_MSG_H

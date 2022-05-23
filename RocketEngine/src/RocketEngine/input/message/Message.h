/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill
	
	This file is based on a similar file 
	from Champlain College's EGP-410 (Game AI) course

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 GameMessage.h
	=========================
	This class is the base message class. 
	It is meant to be built open by various message types (KeyDownMessages, etc). 
	This file contains the enum list of message types as well.

********/

#ifndef MSG_H
#define MSG_H

#include <string>
#include <ostream>
#include "MessageType.h"

namespace RKTEngine
{

	//Event system defines made w/ help from https://github.com/TheCherno/Hazel under the Apache-2.0 license
	#define EVENT_CLASS_TYPE(type)  static MessageType GetStaticType() { return MessageType::type; }\
								virtual MessageType GetMessageType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Message
	{
		friend class MessageManager;

		public:
			/***
				* Constructor requires a GameMessageType parameter
			***/
			Message(const MessageType type)
				:mType(type), mSubmittedTime(0), mScheduledTime(0)
			{
			}

			/***
				* Destructor
			***/
			virtual ~Message() {};

			/***
				* Access the scheduled time for the message
			***/
			double getScheduledTime() const { return mScheduledTime; };

			/***
				* Access the submitted time for the message
			***/
			double getSubmittedTime() const { return mSubmittedTime; };

			virtual MessageType GetMessageType() const = 0;
			virtual const char* GetName() const = 0;
			virtual int GetCategoryFlags() const = 0;
			virtual std::string toString() const { return GetName(); }

			bool processed = false;

		protected:
			MessageType mType;
			double mSubmittedTime;
			double mScheduledTime;
	};

	//Revised from https://github.com/TheCherno/Hazel
	//A game engine and tutorial series developed under the Apache-2.0 license
	class MessageDispatcher
	{
		public:
			MessageDispatcher(Message& message)
				: mMessage(message)
			{
			}

			// F will be deduced by the compiler
			template<typename T, typename F>
			bool dispatch(const F& func)
			{
				if (mMessage.GetMessageType() == T::GetStaticType())
				{
					mMessage.processed |= func(static_cast<T&>(mMessage));
					return true;
				}
				return false;
			}

		private:
			Message& mMessage;
	};

	inline std::ostream& operator<<(std::ostream& os, const Message& msg)
	{
		return os << msg.toString();
	}
}
#endif // !GAME_MSG_H

#ifndef KEY_MSG_H
#define KEY_MSG_H

#include "Message.h"
#include "RocketEngine/input/KeyCodes.h"

namespace RKTEngine
{
	class KeyMessage : public Message
	{
		public:
			KeyMessage(const KeyCode keyCode) :
				mKeyCode(keyCode),
				Message(MessageType::KEY) {};
			
			inline KeyCode getKeyCode() { return mKeyCode; }

			EVENT_CLASS_CATEGORY(MSG_CATEGORY_KEY | MSG_CATEGORY_INPUT)

		protected:
			KeyCode mKeyCode;
	};

	class KeyDownMessage : public KeyMessage
	{
		public:
			KeyDownMessage(const KeyCode keyCode, const int repeatCount) :
							KeyMessage(keyCode), mRepeatCount(repeatCount) {};
			
			EVENT_CLASS_TYPE(KEY_DOWN);

		private:
			int mRepeatCount;
	};


	class KeyUpMessage : public KeyMessage
	{
		public:
			KeyUpMessage(const KeyCode keyCode) :
				KeyMessage(keyCode) {};

			EVENT_CLASS_TYPE(KEY_UP);
		private:
	};

}

#endif // !KEY_MSG_H

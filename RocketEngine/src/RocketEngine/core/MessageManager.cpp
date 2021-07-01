/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	This file is based on a similar file
	from Champlain College's EGP-410 (Game AI) course.

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 GameMessageManager.cpp
	=========================

********/

#include "MessageManager.h"
#include "EngineCore.h"

namespace RKTEngine
{

	MessageManager::MessageManager()
	{
	}

	MessageManager::~MessageManager()
	{
		auto& iter = mMessages.begin();
		while (iter != mMessages.end())
		{
			delete (*iter);
			++iter;
		}
	}

	void MessageManager::addMessage(Message* pMessage, int delay)
	{
		double currentTime = EngineCore::getInstance()->getTime();

		// Set frame timing for message execution
		pMessage->mSubmittedTime = currentTime;
		pMessage->mScheduledTime = currentTime + delay;

		mMessages.push_back(pMessage);
	}

	void MessageManager::processMessagesForThisFrame(float deltaTime)
	{
		double currentTime = EngineCore::getInstance()->getTime();

		auto& iter = mMessages.begin();

		// Execute message if the message has been queued 
		// for length of the messages mScheduledTime.
		while (iter != mMessages.end())
		{
			if ((*iter)->getScheduledTime() <= currentTime)
			{
				mMessageCallback(*(*iter));
				delete (*iter);
				iter = mMessages.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}
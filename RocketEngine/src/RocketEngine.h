#ifndef ROCKET_ENGINE_H
#define ROCKET_ENGINE_H

#include "RocketEngine/core/Log.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/InputSystem.h"
#include "RocketEngine/core/MessageManager.h"
#include "RocketEngine/core/Actor.h"
#include "RocketEngine/input/MessageDefines.h"

using Input = RKTEngine::InputSystem;
namespace KeyCode = RKTEngine::Key;
namespace MouseCode = RKTEngine::Mouse;

namespace RKTEngine
{
	const double FRAME_TIME_60FPS = 16.7;
	const double FRAME_TIME_30FPS = 33.3;
}

#endif // !ROCKET_ENGINE_H

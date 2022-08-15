/********
	=========================
			 ROCKET ENGINE
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This class makes use of the spdlog logging library
	(https://github.com/gabime/spdlog)

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 RK_Log.h
	=========================
	This class is utlized as a static class. It incorporates spdlog
	in order to output info and errors to the console and to log files.

********/

#ifndef RKT_LOG_H
#define RKT_LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <RKTUtils/MemoryTracker.h>

#define LOG_SUBSYS(X) #X " ::"

namespace RKTEngine
{
	class Log
	{
		public:
			/***
				* Initializes loggers for console and file outputs
			***/
			static void initialize();

			/***
				* Report current memory allocations to console and file logs
			***/
			static void reportAllocs();

			/***
				* Cleanup console and file loggers
			***
			void cleanup();*/

			/***
				* Access console logger for engine core
			***/
			static std::shared_ptr<spdlog::logger>& getCoreLogger() { return msCoreLogger; };
			/***
				* Access console logger for game app
			***/
			static std::shared_ptr<spdlog::logger>& getClientLogger() { return msClientLogger; };

		private:
			static std::shared_ptr<spdlog::logger> msCoreLogger, msClientLogger;
	};
}

//Init macro
#define RKT_LOGGER_INIT()		::RKTEngine::Log::initialize();

//Memory leak report macro
#define RKT_MEMREPORT()			::RKTEngine::Log::reportAllocs();

//Assertion wrapper
#define RKT_ASSERT(...)			assert(__VA_ARGS__)

// Core log macros
#define RKT_CORE_TRACE(...)		::RKTEngine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define RKT_CORE_INFO(...)		::RKTEngine::Log::getCoreLogger()->info(__VA_ARGS__)
#define RKT_CORE_WARN(...)		::RKTEngine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define RKT_CORE_ERROR(...)		::RKTEngine::Log::getCoreLogger()->error(__VA_ARGS__)
#define RKT_CORE_CRITICAL(...)	::RKTEngine::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define RKT_TRACE(...)			::RKTEngine::Log::getClientLogger()->trace(__VA_ARGS__)
#define RKT_INFO(...)			::RKTEngine::Log::getClientLogger()->info(__VA_ARGS__)
#define RKT_WARN(...)			::RKTEngine::Log::getClientLogger()->warn(__VA_ARGS__)
#define RKT_ERROR(...)			::RKTEngine::Log::getClientLogger()->error(__VA_ARGS__)
#define RKT_CRITICAL(...)		::RKTEngine::Log::getClientLogger()->critical(__VA_ARGS__)

#endif // !RKT_Log

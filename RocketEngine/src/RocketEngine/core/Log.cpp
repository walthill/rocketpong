/********
	=========================
			 ROCKET ENGINE
	=========================
	File Created By: Walter Hill

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 RK_Log.cpp
	=========================

********/


#include "Log.h"

namespace RKTEngine
{
	std::shared_ptr<spdlog::logger> Log::msCoreLogger;
	std::shared_ptr<spdlog::logger> Log::msClientLogger;

	void Log::initialize()
	{
		std::string coutPattern = "%^[%T] %n >>> %v%$";
		std::string foutPattern = "%^[%T] %n {%l} >>> %v%$";

		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.log", true));

		logSinks[0]->set_pattern(coutPattern);
		logSinks[1]->set_pattern(foutPattern);

		msCoreLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
		spdlog::register_logger(msCoreLogger);
		msCoreLogger->set_level(spdlog::level::trace);
		msCoreLogger->flush_on(spdlog::level::trace);

		msClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(msClientLogger);
		msClientLogger->set_level(spdlog::level::trace);
		msClientLogger->flush_on(spdlog::level::trace);
	}

	void Log::reportAllocs()
	{
		std::vector<std::string> reportList = RKTUtil::MemoryTracker::getInstance()->logAllocs();

		RKT_CORE_INFO(reportList[0]);

		for (auto it = reportList.begin() + 1; it != reportList.end(); ++it)
		{
			RKT_CORE_ERROR(it->c_str());
		}
	}
}
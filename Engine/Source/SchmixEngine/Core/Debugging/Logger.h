#pragma once
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <mutex>

namespace SchmixEngine
{
	enum class LogLevel : uint8_t
	{
		None,
		Success,
		Debug,
		Info,
		Warning,
		Error
	};

	class Logger
	{
	public:
		Logger() = default;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		virtual ~Logger();

		void Log(const std::string& Msg, LogLevel Level, const std::string& Src = "", bool HasSource = false);

		static void GlobalLog(const std::string& Msg, LogLevel Level, const std::string& Src = "", bool HasSource = false);

		static Logger* GetGlobalLogger();
		static void SetGlobalLogger(Logger* Log);

	protected:
		virtual void Print(const std::string& Msg, LogLevel Level) = 0;

	private:
		std::mutex m_LogMutex;

		static Logger* s_GlobalLogger;
	};
}
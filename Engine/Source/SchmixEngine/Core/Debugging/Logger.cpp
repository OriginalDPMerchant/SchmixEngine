#include <SchmixEngine/Core/Debugging/Logger.h>

namespace SchmixEngine
{
	Logger* Logger::s_GlobalLogger = nullptr;

	Logger::~Logger()
	{
		if (s_GlobalLogger == this)
			s_GlobalLogger = nullptr;
	}

	void Logger::Log(const std::string& Msg, LogLevel Level, const std::string& Src, bool HasSource)
	{
		const auto TimeStamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		std::stringstream Output;
		Output << "[" << std::put_time(std::localtime(&TimeStamp), "%Y/%m/%d %I:%M:%S %p") << "]";
		Output << (HasSource ? " [" + Src + "]" : "");
		Output << " " + Msg;

		const std::lock_guard<std::mutex> Lock(m_LogMutex);

		this->Print(Output.str(), Level);
	}

	void Logger::GlobalLog(const std::string& Msg, LogLevel Level, const std::string& Src, bool HasSource)
	{
		Logger* GlobalLogger = GetGlobalLogger();
		
		if (GlobalLogger)
			GlobalLogger->Log(Msg, Level, Src, HasSource);
	}

	Logger* Logger::GetGlobalLogger()
	{
		return s_GlobalLogger;
	}

	void Logger::SetGlobalLogger(Logger* Log)
	{
		s_GlobalLogger = Log;
	}
}
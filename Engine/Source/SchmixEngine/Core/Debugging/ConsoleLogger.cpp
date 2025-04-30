#include <SchmixEngine/Core/Debugging/ConsoleLogger.h>

namespace SchmixEngine
{
	void ConsoleLogger::Print(const std::string& Msg, LogLevel Level)
	{
		std::cout << LogLevelToANSIEscapeCode(Level) + Msg + "\n";
	}

	constexpr const char* ConsoleLogger::LogLevelToANSIEscapeCode(LogLevel MsgLogLevel)
	{
		switch (MsgLogLevel)
		{
		case LogLevel::Success:
			return "\x1b[38;5;46;40m";
		case LogLevel::Debug:
			return "\x1b[38;5;33;40m";
		case LogLevel::Info:
			return "\x1b[38;5;15;40m";
		case LogLevel::Warning:
			return "\x1b[38;5;208;40m";
		case LogLevel::Error:
			return "\x1b[38;5;196;40m";
		default:
			return "\x1b[0m";
		}
	}
}

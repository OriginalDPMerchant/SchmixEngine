#pragma once
#include <SchmixEngine/Core/Debugging/Logger.h>
#include <iostream>

namespace SchmixEngine	
{
	class ConsoleLogger : public Logger
	{
	public:
		ConsoleLogger() = default;

		ConsoleLogger(const ConsoleLogger&) = delete;
		ConsoleLogger& operator=(const ConsoleLogger&) = delete;

		virtual ~ConsoleLogger() = default;

	protected:
		virtual void Print(const std::string& Msg, LogLevel Level) override;

		constexpr const char* LogLevelToANSIEscapeCode(LogLevel MsgLogLevel);
	};
}
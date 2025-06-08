#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#define LOG(Type, FormatString, ...) LogManager::LogFormatted(Type, FormatString, __VA_ARGS__)
enum ELogType
{
	LM_Info,
	LM_Warn,
	LM_Error,
};
struct FLogEntry 
{
	ELogType type;
	std::string messages;
};
static std::vector<FLogEntry> messages;
class LogManager 
{
public:
	template<typename... Args>
	static void LogFormatted(ELogType type, const std::string& formatStr, Args&&... args);
private:
	static void Log(const FLogEntry& LogEntry);
};

template<typename ...Args>
inline void LogManager::LogFormatted(ELogType type, const std::string& formatStr, Args && ...args)
{
	std::ostringstream oss;
	(oss << ... << std::forward<Args>(args)); // fold expression (C++17)
	FLogEntry logEntry;
	logEntry.type = type;
	logEntry.messages = formatStr + oss.str();
	Log(logEntry);
}

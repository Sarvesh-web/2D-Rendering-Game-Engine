#pragma once
#include <vector>
#include <string>
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
class LogManager 
{
public:
	static std::vector<FLogEntry> messages;
	static void Log(FLogEntry LogEntry);
};
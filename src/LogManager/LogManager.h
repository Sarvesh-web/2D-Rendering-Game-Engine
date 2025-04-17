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
static std::vector<FLogEntry> messages;
class LogManager 
{
public:
	static void Log(FLogEntry LogEntry);
};
#include "LogManager.h"
#include <iostream>
#define GREEN "\033[32m"  // Green color for INFO
#define YELLOW "\033[33m"  // Yellow for WARNING
#define RED "\033[31m"    // Red color for ERROR
#define RESET "\033[0m"   // Reset color to default
void LogManager::Log(FLogEntry LogEntry)
{
	// TO DO :
	// Print on the console the messgae : 
	// LOG: [ Date Time ]
	// Get current time
	std::time_t now = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime,&now);

	// Format the timestamp as [YYYY-MM-DD HH:MM:SS]
	char timeBuffer[20];
	std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &localTime);

	// Print the log message
	switch (LogEntry.type)
	{
	case LM_Info:
		std::cout << GREEN << "LOG: [" << timeBuffer << "] " << LogEntry.messages << RESET << std::endl;
		break;
	case LM_Warn:
		std::cout << YELLOW << "WARNING: [" << timeBuffer << "] " << LogEntry.messages << RESET << std::endl;
		break;
	case LM_Error:
		std::cout << RED << "ERROR: [" << timeBuffer << "] " << LogEntry.messages << RESET << std::endl;
		break;
	default:
		break;
	}
	messages.push_back(LogEntry);
}

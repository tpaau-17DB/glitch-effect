#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>

class Logger 
{
    public:
	static void SetVerbosity(const int verbosity);
	static int GetVerbosity();

	static void PrintUsage();
	static void PrintUnsupported(const char *arg);

	static void PrintLog(const std::string message, const int layer);
	static void PrintLog(const std::string message);
	static void PrintWarn(const std::string  message, const int layer);
	static void PrintWarn(const std::string  message);
	static void PrintErr(const std::string message, const int layer);
	static void PrintErr(const std::string message);

    private:
	static int* verbosity;
	static const std::string GREEN; 
	static const std::string YELLOW;
	static const std::string RED;	
	static const std::string RESET;
};

#endif

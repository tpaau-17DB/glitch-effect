#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>

class Logger 
{
    public:
	static void SetVerbosity(const int verbosity);
	static int GetVerbosity();

        static void SetOverrideFiltering(const bool overrideFiltering);
        static bool GetOverrideFiltering();

        static void PrintDebug(const std::string message, const int layer);
	static void PrintDebug(const std::string message);
	static void PrintLog(const std::string message, const int layer);
	static void PrintLog(const std::string message);
	static void PrintWarn(const std::string  message, const int layer);
	static void PrintWarn(const std::string  message);
	static void PrintErr(const std::string message, const int layer);
	static void PrintErr(const std::string message);

    private:
	static int verbosity;
        static bool overrideFiltering;

        static const std::string BLUE;
	static const std::string GREEN; 
	static const std::string YELLOW;
	static const std::string RED;	
	static const std::string RESET;
};

#endif

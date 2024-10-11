#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>

class Logger 
{
    

    public:
        enum LogLevel
        {
         All = 0,
            Standard = 1,
            Quiet = 2,
         ErrorsOnly = 3
        };

        static void SetVerbosity(const Logger::LogLevel logLevel);
	static Logger::LogLevel GetVerbosity();

        static void SetOverrideFiltering(const bool overrideFiltering);
        static bool GetOverrideFiltering();

        static void SetNCursesMode(const bool mode);

        static void PrintDebug(const std::string message, const int layer);
	static void PrintDebug(const std::string message);
	static void PrintLog(const std::string message, const int layer);
	static void PrintLog(const std::string message);
	static void PrintWarn(const std::string  message, const int layer);
	static void PrintWarn(const std::string  message);
	static void PrintErr(const std::string message, const int layer);
	static void PrintErr(const std::string message);

    private:
	static Logger::LogLevel logLevel;
        static bool overrideFiltering;
        static bool ncursesMode;

        static const std::string BLUE;
	static const std::string GREEN; 
	static const std::string YELLOW;
	static const std::string RED;	
	static const std::string RESET;

        static void print(const std::string &message, const int prior, const int layer);
};

#endif

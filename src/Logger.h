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


        // GETTERS
	static Logger::LogLevel GetVerbosity();


        // SETTERS
        static void SetVerbosity(const Logger::LogLevel logLevel);
        static void SetOverrideFiltering(const bool overrideFiltering);
        static void SetNCursesMode(const bool mode);
        static void SetNoColor(const bool nocolor);
        static void SetShowDateTime(const bool enabled);
        static void SetDatetimeFormat(const std::string format);

        
        // PRINTING FUNCTIONS
        static void PrintDebug(const std::string message, const int layer);
	static void PrintDebug(const std::string message);
	static void PrintLog(const std::string message, const int layer);
	static void PrintLog(const std::string message);
	static void PrintWarn(const std::string  message, const int layer);
	static void PrintWarn(const std::string  message);
	static void PrintErr(const std::string message, const int layer);
	static void PrintErr(const std::string message);


    private:
        // INTERNAL FUNCTIONS
        static void print(const std::string &message, const int prior, const int layer);
        static std::string getHeader(const int id);
        static std::string getDateTime();
        static bool isValidDateTimeFormat(const std::string& format);


        // CONSTS
        static const std::string BLUE;
	static const std::string GREEN; 
	static const std::string YELLOW;
	static const std::string RED;	
	static const std::string RESET;


        // DYNAMIC PRIVATE VARIABLES
        static std::string dateTimeFormat;
	static Logger::LogLevel logLevel;
        static bool overrideFiltering;
        static bool ncursesMode;
        static bool nocolor;
        static bool dateTimeEnabled;
};

#endif

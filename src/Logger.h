#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <ctime>

class Logger 
{
    public:
        // PUBLIC STRUCTS
        enum LogLevel
        {
            All = 0,
            Standard = 1,
            Quiet = 2,
            ErrorsOnly = 3
        };


        // GETTERS
	static Logger::LogLevel GetVerbosity();
        static bool GetLogBufferingEnabled();


        // SETTERS
        static void SetVerbosity(const Logger::LogLevel logLevel);
        static void SetVerbosity(const int verbosity);
        static void SetOverrideFiltering(const bool overrideFiltering);
        static void SetNCursesMode(const bool mode);
        static void SetNoColor(const bool nocolor);
        static void SetShowDatetime(const bool enabled);
        static void SetDatetimeFormat(const std::string format);
        static void SetUseLogAccumulation(const bool useLogAccumulation);

        
        // PRINTING FUNCTIONS
	static void PrintDebug(const std::string& message);
	static void PrintDebug(const std::string& message, const bool overrideFiltering);
	static void PrintLog(const std::string& message);
	static void PrintLog(const std::string& message, const bool overrideFiltering);
	static void PrintWarn(const std::string& message);
	static void PrintWarn(const std::string& message, const bool overrideFiltering);
	static void PrintErr(const std::string& message);    
        static void PrintErr(const std::string& message, const bool overrideFiltering); 


        // OTHER PUBLIC FUNCTIONS
        static void ClearLogBufer();
        static void ReleaseLogBuffer();


    private:
        // PRIVATE STRUCTS
        struct BufferedLog
        {
            std::string Message;
            time_t Date;
            short LogLevel;
            bool OverrideFiltering;
        };


        // INTERNAL FUNCTIONS
        static void print(const std::string &message, const int prior, const bool overrideFiltering);
        static std::string getHeader(const int id);
        static std::string getDatetimeHeader(time_t time);
        static bool isValidDatetimeFormat(const std::string& format);
        static std::string colorify(const std::string& color, const std::string& toColorify);
        static std::string logLevelToColor(const unsigned short logLevel);


        // CONSTS
        static const std::string BLUE;
	static const std::string GREEN; 
	static const std::string YELLOW;
	static const std::string RED;	
	static const std::string RESET;


        // DYNAMIC PRIVATE VARIABLES
        static std::vector<BufferedLog> logBuffer;
        static std::string dateTimeFormat;
	static Logger::LogLevel logLevel;
        static bool overrideFiltering;
        static bool ncursesMode;
        static bool nocolor;
        static bool dateTimeEnabled;
        static bool useLogAccumulation;
};

#endif

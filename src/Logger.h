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

        struct BufferedLog
        {
            std::string Message = "E";
            time_t Date;
            short LogLevel = 0;
            bool OverrideFiltering = false;
            bool IsRaw = false;
        };

        // GETTERS
	    static Logger::LogLevel GetVerbosity();


        // SETTERS
        static void SetVerbosity(const LogLevel verbosity);
        static void SetVerbosity(const int verbosity);

        static void SetOverrideFiltering(const bool enabled);
        static void SetNCursesMode(const bool enabled);

        static void SetNoColor(const bool enabled);

        static void SetShowDatetime(const bool enabled);
        static void SetDatetimeFormat(const std::string format);

        static void SetLogFilePath(const std::string& path);
        static void ToggleFileLogging(const bool enabled);

        
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
        static void ReleaseLogBuffer();
        static void WriteToBuffer(const std::string& str);
        static void WriteLogToBuffer(const BufferedLog& log);
};

#endif

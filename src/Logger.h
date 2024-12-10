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


    private:
        // INTERNAL FUNCTIONS
        static void print(const std::string &message, const int prior, const bool overrideFiltering);
        static std::string getHeader(const int id);

        static void clearLogBufer();

        static std::string getDatetimeHeader(time_t time);
        static bool isValidDatetimeFormat(const std::string& format);

        static std::string colorify(const std::string& color, const std::string& toColorify);

        static std::string logLevelToColor(const unsigned short logLevel);

        static void disableFileLoggingOnError();
        static int overwriteFile(const std::string& filePath, const std::string& contents);
        static int appendToFile(const std::string& filePath, const std::string& contents);
        static int writeToFile(const std::string& filePath, const std::string& contents, const bool overwrite);

        static std::string expandPath(const std::string& path);

        // CONSTANTS
        static const std::string BLUE;
        static const std::string GREEN; 
        static const std::string YELLOW;
        static const std::string RED;	
        static const std::string RESET;


        // DYNAMIC PRIVATE VARIABLES
        static std::vector<BufferedLog> logBuffer;
        static unsigned int maxLogBufferSize;

        static std::string dateTimeFormat;
        static bool dateTimeEnabled;

        static std::string logFilePath;
        static bool fileLoggingEnabled;

        static Logger::LogLevel logLevel;
        static bool overrideFiltering;

        static bool ncursesMode;

        static bool nocolor;
};

#endif

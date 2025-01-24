#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <string>
#include <vector>
#include <ctime>

// PUBLIC STRUCTS
enum LogLevel
{
    Verbose = 0,
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
LogLevel GetLoggerVerbosity();


// SETTERS
void SetLoggerVerbosity(const LogLevel verbosity);
void SetLoggerVerbosity(const int verbosity);

void ToggleLogFilteringOverride(const bool enabled);

void ToggleLogColor(const bool enabled);

void SetMaxLogBufferSize(const int limit);

void ToggleLogDatetime(const bool enabled);
void SetLogDatetimeFormat(const std::string format);

void SetLogFilePath(const std::string& path);
void ToggleFileLogging(const bool enabled);

void SetLogOutputStream(std::ostringstream* stream);
void ToggleUseCustomOutputStream(const bool enabled);


// PRINTING FUNCTIONS
void PrintDebug(const std::string& message);
void PrintDebug(const std::string& message, const bool overrideFiltering);

void PrintLog(const std::string& message);
void PrintLog(const std::string& message, const bool overrideFiltering);

void PrintWarn(const std::string& message);
void PrintWarn(const std::string& message, const bool overrideFiltering);

void PrintErr(const std::string& message);
void PrintErr(const std::string& message, const bool overrideFiltering); 

void PrintCrit(const std::string& message);
void PrintCrit(const std::string& message, const bool overrideFiltering); 

// Other public methods
void FlushLogBuffer();
void FlushLogStream();

#endif

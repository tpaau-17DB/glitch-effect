#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "Logger.h"

using namespace std;


// Constants
const string BLUE = "\033[34m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string RED = "\033[31m";
const string MAGENTA = "\033[35m";
const string RESET = "\033[0m"; 


// Variables
vector<BufferedLog> logs;
int maxLogBufferSize = 128;

string dateTimeFormat = "%H:%M:%S";
bool dateTimeEnabled = false;

string logFilePath = "";
bool fileLoggingEnabled;

LogLevel logLevel = Standard;
bool overrideFilteringGlobal = false;

bool nocolor = false;

bool useCustomLogStream = false;
ostringstream* logStream = nullptr;

// Internal methods
string colorify(const string& color, const string& toColorify)
{
    return color + toColorify + RESET;
}

string logLevelToColor(const unsigned short logLevel)
{
    switch(logLevel)
    {
        case 0:
            return BLUE;

        case 1:
            return GREEN;

        case 2:
            return YELLOW;

        case 3:
            return RED;

        case 4:
            return MAGENTA;


        default:
            PrintErr("Unknown logLevel value!");
            return "";
    }
}

string getHeader(const int id)
{
    string header;

    switch(id)
    {
        case 0:
            header = "[DEB] ";
            break;

        case 1:
            header = "[LOG] ";
            break;

        case 2:
            header = "[WARN] ";
            break;

        case 3:
            header = "[ERR] ";
            break;

        case 4:
            header = "[CRIT] ";
            break;
    };

    if(!nocolor)
    {
        header = colorify(logLevelToColor(id), header);
    }

    return header;
}

string getDatetimeHeader(time_t t)
{
    tm* now = localtime(&t);

    if (!now)
    {
        if (!nocolor)
        {
            return RED + "[ERROR: NULL POINTER] " + RESET;
        }
        else
        {
            return "[ERROR: NULL POINTER] ";
        }
    }

    char buffer[80];

    if (strftime(buffer, sizeof(buffer),
                dateTimeFormat.c_str(), now) == 0) 
    {
        if (!nocolor)
        {
            return RED + "[FORMAT ERROR] " + RESET;
        }
        else
        {
            return "[FORMAT ERROR] ";
        }
    }

    return "[" + string(buffer) + "] ";
}

bool isValidDatetimeFormat(const string& format) 
{
    time_t t = time(0);
    tm* now = localtime(&t);
    
    if (!now) 
    {
        return false;
    }

    char buffer[80];
    if (strftime(buffer, sizeof(buffer),
                format.c_str(), now) == 0) 
    {
        return false;
    }

    return true;
}

string expandPath(const string& path)
{
    string fullPath = path;

    if (!fullPath.empty() && fullPath[0] == '~') 
    {
        string homeDir = getenv("HOME");
        return homeDir + fullPath.substr(1);
    } 
    else 
    {
        return fullPath;
    }
}

int writeToFile(const string& filePath, const string& contents, const bool overwrite)
{
    string expandedPath = expandPath(filePath);
    ofstream outputFile;

    if (overwrite)
    {
       outputFile.open(expandedPath);
    }
    else
    {
        outputFile.open(expandedPath, ios::app);
    }

    if(!outputFile)
    {
        PrintErr("Unable to open file '" + filePath + "' for writing!");
        ToggleFileLogging(false);
        return 1;
    }

    outputFile<<contents;
    outputFile.flush();
    outputFile.close();

    return 0;
}

int overwriteFile(const string& filePath, const string& contents)
{
    return writeToFile(filePath, contents, true);
}

int appendToFile(const string& filePath, const string& contents)
{
    return writeToFile(filePath, contents, false);
}

string formatLog(BufferedLog& log)
{
    if (log.IsRaw)
    {
        return log.Message + "\n";
    }
    else
    {
        if (dateTimeEnabled)
        {
            return getHeader(log.LogLevel) + getDatetimeHeader(log.Date) + log.Message + "\n";
        }
        else
        {
            return getHeader(log.LogLevel) + log.Message + "\n";
        }
    }
}

void print(const string& message, const int prior, const bool overrideFiltering)
{
    BufferedLog log;
    log.Message = message;
    log.LogLevel = prior;
    log.OverrideFiltering = overrideFiltering;
    log.Date = time(0);

    if (log.LogLevel >= logLevel || overrideFilteringGlobal || log.OverrideFiltering)
    {
        if (!useCustomLogStream)
        {
            clog<<formatLog(log);
        }
        else if (logStream)
        {
            *logStream<<formatLog(log);
        }
        else
        {
            PrintErr("logStream is a nullptr! Falling back to default stream.");
            ToggleUseCustomOutputStream(false);
        }

        if (fileLoggingEnabled)
        {
            logs.push_back(log);
            if (logs.size() > (unsigned int)maxLogBufferSize)
            {
                FlushLogBuffer();
            }
        }
    }
}


// Setters
void SetLoggerVerbosity(const LogLevel verbosity) 
{
    logLevel = verbosity;
}

void SetLoggerVerbosity(const int verbosity)
{
    if (!(verbosity >= 0 && verbosity <= 3))
    {
        PrintErr("Verbosity value must be between 0 and 3!");
        return;
    }
    logLevel = LogLevel(verbosity);
}

void ToggleLogFilteringOverride(const bool enabled)
{
    overrideFilteringGlobal = enabled;
}

void ToggleLogColor(const bool enabled)
{
    nocolor = enabled;
}

void SetMaxLogBufferSize(const int maxSize)
{
    if (maxSize <= 0)
    {
        PrintErr("Cannot set max buffer size to a value that is less or equal to zero!");
    }
    else
    {
        maxLogBufferSize = maxSize;
    }
}

void ToggleLogDatetime(const bool enabled)
{
    dateTimeEnabled = enabled;
}

void SetLogDatetimeFormat(const string format)
{
    if (isValidDatetimeFormat(format))
        dateTimeFormat = format;
    else
        PrintErr("Invalid datetime format specified: '" + format + "'.");
}

void SetLogFilePath(const string& path)
{
    if (overwriteFile(path, "[OUTPUT START]\n") == 0)
    {
        logFilePath = path;
    }
    else
    {
        PrintErr("Error opening file.");
    }
}

void ToggleFileLogging(const bool enabled)
{
    fileLoggingEnabled = enabled;
}

void SetLogOutputStream(ostringstream* stream)
{
    if (!stream)
    {
        PrintErr("Stream pointer is invalid!");
    }
    else
    {
        logStream = stream;
    }
}

void ToggleUseCustomOutputStream(const bool enabled)
{
    useCustomLogStream = enabled;
}


// Getters
LogLevel GetLoggerVerbosity()
{
    return logLevel;
}


// Log methods
void PrintDebug(const string& message)
{
    print(message, 0, false);
}

void PrintDebug(const string& message, const bool overrideFiltering)
{
    print(message, 0, overrideFiltering);
}

void PrintLog(const string& message)
{
    print(message, 1, false);
}

void PrintLog(const string& message, const bool overrideFiltering)
{
    print(message, 1, overrideFiltering);
}

void PrintWarn(const string& message)
{
    print(message, 2, false);
}

void PrintWarn(const string& message, const bool overrideFiltering)
{
    print(message, 2, overrideFiltering);
}

void PrintErr(const string& message)
{
    print(message, 3, false);
}

void PrintErr(const string& message, const bool overrideFiltering)
{
    print(message, 3, overrideFiltering);
}

void PrintCrit(const string& message)
{
    print(message, 4, false);
}

void PrintCrit(const string& message, const bool overrideFiltering)
{
    print(message, 4, overrideFiltering);
}

// Other public methods
void FlushLogBuffer()
{
    ostringstream logStream;

    for (BufferedLog& log : logs)
    {
        logStream<<formatLog(log);
    }

    appendToFile(logFilePath, logStream.str());
    logs = vector<BufferedLog>();
}

void FlushLogStream()
{
    if (logStream)
    {
        clog<<logStream->str();
        *logStream = ostringstream();
    }
    else
    {
        PrintErr("logStream is a nullptr! Falling back to default stream.");
        ToggleUseCustomOutputStream(false);
    }
}

#include <iostream>
#include <ncurses.h>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "Logger.h"

using namespace std;


// Constants
const string BLUE = "\033[34m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string RED = "\033[31m";
const string RESET = "\033[0m"; 


// Init
vector<Logger::BufferedLog> logBuffer = vector<Logger::BufferedLog>();
unsigned int maxLogBufferSize = 100;

string dateTimeFormat = "%H:%M:%S";
bool dateTimeEnabled = false;

string logFilePath = "";
bool fileLoggingEnabled;

Logger::LogLevel logLevel = Logger::Standard;
bool overrideFiltering = false;

bool ncursesMode = false;

bool nocolor = false;

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

        default:
            Logger::PrintErr("Unknown logLevel value!");
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
            header = "[WAR] ";
            break;

        case 3:
            header = "[ERR] ";
            break;
    };

    if(!nocolor)
    {
        header = colorify(logLevelToColor(id), header);
    }

    return header;
}

void print(const string &message, const int prior, const bool overrideFiltering)
{
    Logger::BufferedLog bufferedLog;
    bufferedLog.Message = message;
    bufferedLog.LogLevel = prior;
    bufferedLog.Date = time(0);
    logBuffer.push_back(bufferedLog);

    if (logBuffer.size() >= maxLogBufferSize)
    {
        Logger::ReleaseLogBuffer();
    }
}

void clearLogBufer()
{
    logBuffer = vector<Logger::BufferedLog>();

    Logger::BufferedLog bufferedLog;

    bufferedLog.Message = "[BUFFER CLEARED]";
    bufferedLog.LogLevel = 0;
    bufferedLog.OverrideFiltering = true;
    bufferedLog.Date = time(0);

    logBuffer.push_back(bufferedLog);
}

string getDatetimeHeader(time_t t)
{
    tm* now = localtime(&t);

    if (!now)
    {
        if (!nocolor)
            return RED + "[ERROR: NULL POINTER] " + RESET;
        else
            return "[ERROR: NULL POINTER] ";
    }

    char buffer[80];

    if (strftime(buffer, sizeof(buffer), dateTimeFormat.c_str(), now) == 0) 
    {
        if (!nocolor)
            return RED + "[FORMAT ERROR] " + RESET;
        else
            return "[FORMAT ERROR] ";
    }

    return "[" + string(buffer) + "] ";
}

bool isValidDatetimeFormat(const std::string& format) 
{
    time_t t = time(0);
    tm* now = localtime(&t);
    
    if (!now) 
    {
        return false;
    }

    char buffer[80];
    if (strftime(buffer, sizeof(buffer), format.c_str(), now) == 0) 
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
       outputFile.open(expandedPath);
    else
        outputFile.open(expandedPath, ios::app);

    if(!outputFile)
    {
        Logger::PrintErr("Unable to open file '" + filePath + "' for writing!");
        Logger::ToggleFileLogging(false);
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

// Setters
void Logger::SetVerbosity(const LogLevel verbosity) 
{
    logLevel = verbosity;
}

void Logger::SetVerbosity(const int verbosity)
{
    if (!(verbosity >= 0 && verbosity <= 3))
    {
        PrintErr("Verbosity value must be between 0 and 3!");
        return;
    }
    logLevel = LogLevel(verbosity);
}

void Logger::SetOverrideFiltering(const bool enabled)
{
    overrideFiltering = enabled;
}

void Logger::SetNCursesMode(const bool enabled)
{
    ncursesMode = enabled;
}

void Logger::SetNoColor(const bool enabled)
{
    nocolor = enabled;
}

void Logger::SetShowDatetime(const bool enabled)
{
    dateTimeEnabled = enabled;
}

void Logger::SetDatetimeFormat(const string format)
{
    if (isValidDatetimeFormat(format))
        dateTimeFormat = format;
    else
        PrintErr("Invalid datetime format specified: '" + format + "'.");
}

void Logger::SetLogFilePath(const string& path)
{
    if (overwriteFile(path, "[OUTPUT START]\n") == 0)
    {
        logFilePath = path;
    }
    else
    {
        PrintErr("File path cannot be set. Error opening file.");
    }
}

void Logger::ToggleFileLogging(const bool enabled)
{
    fileLoggingEnabled = enabled;
}


// Getters
Logger::LogLevel Logger::GetVerbosity()
{
    return logLevel;
}

// Log methods
void Logger::PrintDebug(const string& message)
{
    print(message, 0, false);
}

void Logger::PrintDebug(const string& message, const bool overrideFiltering)
{
    print(message, 0, overrideFiltering);
}

void Logger::PrintLog(const string& message)
{
    print(message, 1, false);
}

void Logger::PrintLog(const string& message, const bool overrideFiltering)
{
    print(message, 1, overrideFiltering);
}

void Logger::PrintWarn(const string& message)
{
    print(message, 2, false);
}

void Logger::PrintWarn(const string& message, const bool overrideFiltering)
{
    print(message, 2, overrideFiltering);
}

void Logger::PrintErr(const string& message)
{
    print(message, 3, false);
}

void Logger::PrintErr(const string& message, const bool overrideFiltering)
{
    print(message, 3, overrideFiltering);
}

// Other public methods


void Logger::ReleaseLogBuffer()
{
    if (logBuffer.size() == 0)
        return;

    ostringstream logs;
    if (ncursesMode) endwin();

    for (const BufferedLog& log : logBuffer)
    {
        if (logLevel > log.LogLevel && !overrideFiltering && !log.OverrideFiltering) continue;
        if (log.IsRaw)
        {
            logs<<log.Message<<"\n";
        }
        else
        {
            if (dateTimeEnabled)
                logs<<getHeader(log.LogLevel)<<getDatetimeHeader(log.Date)<<log.Message<<"\n";
            else
                logs<<getHeader(log.LogLevel)<<log.Message<<"\n";
        }
    }

    cout<<logs.str();
    if (fileLoggingEnabled)
    {
        appendToFile(logFilePath, logs.str());
    }

    clearLogBufer();
}

void Logger::WriteToBuffer(const string& str)
{
    BufferedLog log;
    log.Message = str;
    log.IsRaw = true;
    logBuffer.push_back(log);
}

void Logger::WriteLogToBuffer(const BufferedLog& log)
{
    logBuffer.push_back(log);
}

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
const string Logger::BLUE = "\033[34m";
const string Logger::GREEN = "\033[32m";
const string Logger::YELLOW = "\033[33m";
const string Logger::RED = "\033[31m";
const string Logger::RESET = "\033[0m"; 


// Init
vector<Logger::BufferedLog> Logger::logBuffer = vector<Logger::BufferedLog>();
unsigned int Logger::maxLogBufferSize = 100;

string Logger::dateTimeFormat = "%H:%M:%S";
bool Logger::dateTimeEnabled = false;

string Logger::logFilePath = "";
bool Logger::fileLoggingEnabled;

Logger::LogLevel Logger::logLevel = Logger::Standard;
bool Logger::overrideFiltering = false;

bool Logger::ncursesMode = false;

bool Logger::nocolor = false;


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


// Internal methods
string Logger::getHeader(const int id)
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

void Logger::print(const string &message, const int prior, const bool overrideFiltering)
{
    BufferedLog bufferedLog;
    bufferedLog.Message = message;
    bufferedLog.LogLevel = prior;
    bufferedLog.Date = time(0);
    logBuffer.push_back(bufferedLog);

    if (logBuffer.size() >= maxLogBufferSize)
    {
        ReleaseLogBuffer();
    }
}

void Logger::clearLogBufer()
{
    logBuffer = vector<Logger::BufferedLog>();

    BufferedLog bufferedLog;

    bufferedLog.Message = "[BUFFER CLEARED]";
    bufferedLog.LogLevel = 0;
    bufferedLog.OverrideFiltering = true;
    bufferedLog.Date = time(0);

    logBuffer.push_back(bufferedLog);
}

string Logger::getDatetimeHeader(time_t t)
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

bool Logger::isValidDatetimeFormat(const std::string& format) 
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

string Logger::colorify(const string& color, const string& toColorify)
{
    return color + toColorify + RESET;
}

string Logger::logLevelToColor(const unsigned short logLevel)
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
            PrintErr("Unknown logLevel value!");
            return "";
    }
}

int Logger::overwriteFile(const string& filePath, const string& contents)
{
    return writeToFile(filePath, contents, true);
}

int Logger::appendToFile(const string& filePath, const string& contents)
{
    return writeToFile(filePath, contents, false);
}

int Logger::writeToFile(const string& filePath, const string& contents, const bool overwrite)
{
    string expandedPath = expandPath(filePath);
    ofstream outputFile;

    if (overwrite)
       outputFile.open(expandedPath);
    else
        outputFile.open(expandedPath, ios::app);

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

string Logger::expandPath(const string& path)
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

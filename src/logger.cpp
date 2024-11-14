#include "Logger.h"

#include <iostream>
#include <ncurses.h>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


// Constants
const string Logger::BLUE = "\033[34m";
const string Logger::GREEN = "\033[32m";
const string Logger::YELLOW = "\033[33m";
const string Logger::RED = "\033[31m";
const string Logger::RESET = "\033[0m"; 


// Init
vector<Logger::BufferedLog> Logger::logBuffer = vector<Logger::BufferedLog>();

string Logger::dateTimeFormat = "%H:%M:%S";

Logger::LogLevel Logger::logLevel = Logger::Standard;

bool Logger::overrideFiltering = false;
bool Logger::ncursesMode = false;
bool Logger::nocolor = false;
bool Logger::dateTimeEnabled = false;
bool Logger::useLogAccumulation = false;


// Setters
void Logger::SetVerbosity(const Logger::LogLevel logLevel) 
{
    Logger::logLevel = logLevel;
}

void Logger::SetVerbosity(const int verbosity)
{
    bool cond = verbosity >= 0 && verbosity <= 3;
    if (!cond)
    {
        Logger::PrintErr("Verbosity value must be between 0 and 3!");
        return;
    }
    Logger::logLevel = Logger::LogLevel(verbosity);
}

void Logger::SetOverrideFiltering(const bool overrideFiltering)
{
    Logger::overrideFiltering = overrideFiltering;
}

void Logger::SetNCursesMode(const bool mode)
{
    Logger::ncursesMode = mode;
}

void Logger::SetNoColor(const bool nocolor)
{
    Logger::nocolor = nocolor;
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
        Logger::PrintErr("Invalid datetime format specified: '" + format + "'.");
}

void Logger::SetUseLogAccumulation(const bool useLogAccumulation)
{
    Logger::useLogAccumulation = useLogAccumulation;
}


// Getters
Logger::LogLevel Logger::GetVerbosity()
{
    return Logger::logLevel;
}

bool Logger::GetLogBufferingEnabled()
{
    return useLogAccumulation;
}


// Log methods
void Logger::PrintDebug(const string& message)
{
    Logger::print(message, 0, false);
}

void Logger::PrintDebug(const string& message, const bool overrideFiltering)
{
    Logger::print(message, 0, overrideFiltering);
}

void Logger::PrintLog(const string& message)
{
    Logger::print(message, 1, false);
}

void Logger::PrintLog(const string& message, const bool overrideFiltering)
{
    Logger::print(message, 1, overrideFiltering);
}

void Logger::PrintWarn(const string& message)
{
    Logger::print(message, 2, false);
}

void Logger::PrintWarn(const string& message, const bool overrideFiltering)
{
    Logger::print(message, 2, overrideFiltering);
}

void Logger::PrintErr(const string& message)
{
    Logger::print(message, 3, false);
}

void Logger::PrintErr(const string& message, const bool overrideFiltering)
{
    Logger::print(message, 3, overrideFiltering);
}

// Other public methods
void Logger::ClearLogBufer()
{
    logBuffer = vector<Logger::BufferedLog>();

    Logger::BufferedLog bufferedLog;

    bufferedLog.Message = "CLEARED BUFFER";
    bufferedLog.LogLevel = 0;
    bufferedLog.OverrideFiltering = true;

    logBuffer.push_back(bufferedLog);
}

void Logger::ReleaseLogBuffer()
{
    if (logBuffer.size() == 0)
        return;

    ostringstream stream;

    if (ncursesMode) endwin();

    if (dateTimeEnabled)
    {
        for (const Logger::BufferedLog& log : logBuffer)
        {
            if (logLevel > log.LogLevel && !Logger::overrideFiltering && !log.OverrideFiltering) continue;
            stream<<getHeader(log.LogLevel)<<getDatetimeHeader(log.Date)<<log.Message<<"\n";
        }
    }
    else
    {
        for (const Logger::BufferedLog& log : logBuffer)
        {
            if (logLevel > log.LogLevel && !Logger::overrideFiltering && !log.OverrideFiltering) continue;
            stream<<getHeader(log.LogLevel)<<log.Message<<"\n";
        }
    }

    printf("%s", stream.str().c_str());
    fflush(stdout);
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
    if (useLogAccumulation)
    {
        Logger::BufferedLog bufferedLog;
        bufferedLog.Message = message;
        bufferedLog.LogLevel = prior;
        bufferedLog.Date = time(0);

        logBuffer.push_back(bufferedLog);
    }
    else
    {
        if (prior < logLevel && !overrideFiltering && !Logger::overrideFiltering) return;
        string header = getHeader(prior);
        if (ncursesMode)
        {
            endwin();

            if (dateTimeEnabled)
            {
                string dateHeader = getDatetimeHeader(time(0));
                printf("%s\n", (header + dateHeader + message).c_str());
            }
            else
            {
                printf("%s\n", (header + message).c_str());
            }
            fflush(stdout);
        }
        else
        {
            if (dateTimeEnabled)
            {
                string dateHeader = getDatetimeHeader(time(0));
                cout<<header<<dateHeader<<message<<endl;
            }
            else
            {
                cout<<header<<message<<endl;
            }
        }
    }
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
            Logger::PrintErr("Unknown logLevel value!");
            return "";
    }
}

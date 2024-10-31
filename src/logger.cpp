#include "Logger.h"

#include <iostream>
#include <ncurses.h>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


// Consts
const string Logger::BLUE = "\033[34m";
const string Logger::GREEN = "\033[32m";
const string Logger::YELLOW = "\033[33m";
const string Logger::RED = "\033[31m";
const string Logger::RESET = "\033[0m"; 


// Init
vector<string> Logger::logBuffer = vector<string>();

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

void Logger::SetShowDateTime(const bool enabled)
{
    dateTimeEnabled = enabled;
}

void Logger::SetDatetimeFormat(const string format)
{
    if (isValidDateTimeFormat(format))
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
    logBuffer = vector<string>();
    if (!nocolor)
    {
        logBuffer.push_back(BLUE + "[CLEARED]" + RESET + "\n");
    }
    else
    {
        logBuffer.push_back("[CLEARED]\n");
    }
}

void Logger::ReleaseLogBuffer()
{
    if (logBuffer.size() == 0)
        return;

    if (ncursesMode)
    {
        endwin();
        ostringstream stream;
        for (const string& log : logBuffer)
        {
            stream<<log;
        }
        printf("%s", stream.str().c_str());
        fflush(stdout);
    }
    else
    {
        ostringstream stream;
        for (const string& log : logBuffer)
        {
            stream<<log;
        }
        cout<<stream.str();
    }
}

void Logger::WriteToBuffer(const string& str)
{
    logBuffer.push_back(str);
}


// Internal methods
string Logger::getHeader(const int id)
{
    string header;

    if (!nocolor)
    {
        switch(id)
        {
            case 0:
                header = BLUE + "[DEB] " + RESET;
                break;

            case 1:
                header = GREEN + "[LOG] " + RESET;
                break;

            case 2:
                header = YELLOW + "[WAR] " + RESET;
                break;

            case 3:
                header = RED + "[ERR] " + RESET;
                break;
        };
    }
    else
    {
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
    }

    if (dateTimeEnabled)
        header += getDateTime();

    return header;
}

void Logger::print(const string &message, const int prior, const bool overrideFiltering)
{   
    if (logLevel > prior && !overrideFiltering && !Logger::overrideFiltering) return;
    
    string header = getHeader(prior);

    if (useLogAccumulation)
    {
        logBuffer.push_back(header + message + "\n");
    }
    else
    {
        if (ncursesMode)
        {
            endwin();
            printf("%s\n", (header + message).c_str());
            fflush(stdout);
        }
        else
        {
            cout<<header<<message<<endl;
        }
    }
}

string Logger::getDateTime()
{
    time_t t = time(0);
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

bool Logger::isValidDateTimeFormat(const std::string& format) 
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

#include "Logger.h"

#include <iostream>
#include <ncurses.h>
#include <cstdio>
#include <ctime>
#include <string>

using namespace std;


// Consts
const string Logger::BLUE = "\033[34m";
const string Logger::GREEN = "\033[32m";
const string Logger::YELLOW = "\033[33m";
const string Logger::RED = "\033[31m";
const string Logger::RESET = "\033[0m"; 


// Init
string Logger::dateTimeFormat = "%H:%M:%S";

Logger::LogLevel Logger::logLevel = Logger::Standard;

bool Logger::overrideFiltering = false;
bool Logger::ncursesMode = false;
bool Logger::nocolor = false;
bool Logger::dateTimeEnabled = false;


// Setters
void Logger::SetVerbosity(const Logger::LogLevel logLevel) 
{
    Logger::logLevel = logLevel;
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


// Getters
Logger::LogLevel Logger::GetVerbosity()
{
    return Logger::logLevel;
}


// Log methods
void Logger::PrintDebug(const string message, const int layer)
{
    Logger::print(message, 0, layer);
}

void Logger::PrintDebug(const string message)
{
    Logger::print(message, 0, 0);
}

void Logger::PrintLog(const string message, const int layer)
{
    Logger::print(message, 1, layer);
}

void Logger::PrintLog(const string message)
{
    Logger::print(message, 1, 0);
}

void Logger::PrintWarn(const string message, const int layer)
{
    Logger::print(message, 2, layer);
}

void Logger::PrintWarn(const string message)
{
    Logger::print(message, 2, 0);
}

void Logger::PrintErr(const string message, const int layer)
{
    Logger::print(message, 3, layer);
}

void Logger::PrintErr(const string message)
{
    Logger::print(message, 3, 0);
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

void Logger::print(const string &message, const int prior, const int layer)
{   
    if (logLevel > prior && !overrideFiltering) return;
    
    string spaces = string(layer * 2, ' ');
    string header = getHeader(prior);

    if (ncursesMode)
    {
        endwin();
        printf("%s\n", (spaces + header + message).c_str());
        fflush(stdout);
    }
    else
    {
        cout<<spaces<<header<<message<<endl;
    }
}

string Logger::getDateTime()
{
    time_t t = time(0);
    tm* now = localtime(&t);

    if (!now)
    {
        if (!nocolor)
            return RED + "[error: null pointer] " + RESET;
        else
            return "[error: null pointer] ";
    }

    char buffer[80];

    if (strftime(buffer, sizeof(buffer), dateTimeFormat.c_str(), now) == 0) 
    {
        if (!nocolor)
            return RED + "[format error] " + RESET;
        else
            return RED + "[format error] " + RESET;
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

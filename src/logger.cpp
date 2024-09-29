#include "Logger.h"

#include <iostream>

using namespace std;


// Consts
const string Logger::BLUE = "\033[34m";
const string Logger::GREEN = "\033[32m";
const string Logger::YELLOW = "\033[33m";
const string Logger::RED = "\033[31m";
const string Logger::RESET = "\033[0m"; 


// Init
int Logger::verbosity = 1;
bool Logger::overrideFiltering = false;


// Setters
void Logger::SetVerbosity(const int verbosity) 
{
    if (verbosity >= 0 && verbosity < 4)
        Logger::verbosity = verbosity;
    else
        Logger::PrintErr("Verbsity value must be between 0 and 3!");
}

void Logger::SetOverrideFiltering(const bool overrideFiltering)
{
    Logger::overrideFiltering = overrideFiltering;
}


// Getters
int Logger::GetVerbosity()
{
    return Logger::verbosity;
}

bool Logger::GetOverrideFiltering()
{
    return Logger::overrideFiltering;
}


// Log methods
void Logger::PrintDebug(const string message, const int layer)
{
    if (verbosity != 0 && !overrideFiltering) return;
	cout<<string(layer * 2, ' ')<<GREEN<<"[DEB] "<<RESET<<message<<endl;
}

void Logger::PrintDebug(const string message)
{
    if (verbosity != 0 && !overrideFiltering) return;
	cout<<GREEN<<"[DEB] "<<RESET<<message<<endl;
}

void Logger::PrintLog(const string message, const int layer)
{
    if (verbosity > 1 && !overrideFiltering) return;
	cout<<string(layer * 2, ' ')<<GREEN<<"[LOG] "<<RESET<<message<<endl;
}

void Logger::PrintLog(const string message)
{
    if (verbosity > 1 && !overrideFiltering) return;
	cout<<GREEN<<"[LOG] "<<RESET<<message<<endl;
}

void Logger::PrintWarn(const string message, const int layer)
{
    if (verbosity > 2 && !overrideFiltering) return;
	cout<<string(layer * 2, ' ')<<YELLOW<<"[WAR] "<<RESET<<message<<endl;
}

void Logger::PrintWarn(const string message)
{
    if (verbosity > 2 && !overrideFiltering) return;
	cout<<YELLOW<<"[WAR] "<<RESET<<message<<endl;
}

void Logger::PrintErr(const string message, const int layer)
{
    cout<<string(layer * 2, ' ')<<RED<<"[ERR] "<<RESET<<message<<endl;
}

void Logger::PrintErr(const string message)
{
    cout<<RED<<"[ERR] "<<RESET<<message<<endl;
}

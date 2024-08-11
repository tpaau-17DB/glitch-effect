#include "Logger.h"

#include <iostream>

using namespace std;

const string Logger::GREEN = "\033[32m";
const string Logger::YELLOW = "\033[33m";
const string Logger::RED = "\033[31m";
const string Logger::RESET = "\033[0m"; 

void Logger::SetVerbosity(const int verbosity) 
{
    Logger::verbosity = new int(verbosity);
}

int Logger::GetVerbosity()
{
    return *Logger::verbosity;
}

void Logger::PrintUsage()
{
    cout << "Usage: glitch [options]" << endl;
    cout << "  --help, -h: Display this help message" << endl;
    cout << "  -x <int>: Manually set X offset, disables autocenter" << endl;
    cout << "  -y <int>: Manually set Y offset, disables autocenter" << endl;
    cout << "  -c <path>: specify config file path" << endl;
    cout << "  -i <path>: specify the ascii file to display" << endl;
    cout << "  -v <int>: Set log verbosity" << endl;
}

void Logger::PrintUnsupported(const char *arg)
{
    PrintLog("Unsupported use of an argument: " + string(arg));
    Logger::PrintUsage();
}

void Logger::PrintLog(const string message, const int layer)
{
    if (verbosity && *verbosity > 0) return;
	cout<<string(layer * 2, ' ')<<GREEN<<"[LOG] "<<RESET<<message<<endl;
}

void Logger::PrintLog(const string message)
{
    if (verbosity && *verbosity > 0) return;
	cout<<GREEN<<"[LOG] "<<RESET<<message<<endl;
}

void Logger::PrintWarn(const string message, const int layer)
{
    if (verbosity && *verbosity > 1) return;
	cout<<string(layer * 2, ' ')<<YELLOW<<"[WARN] "<<RESET<<message<<endl;
}

void Logger::PrintWarn(const string message)
{
    if (verbosity && *verbosity > 1) return;
	cout<<YELLOW<<"[WARN] "<<RESET<<message<<endl;
}

void Logger::PrintErr(const string message, const int layer)
{
    cout<<string(layer * 2, ' ')<<RED<<"[ERR] "<<RESET<<message<<endl;
}

void Logger::PrintErr(const string message)
{
    cout<<RED<<"[ERR] "<<RESET<<message<<endl;
}

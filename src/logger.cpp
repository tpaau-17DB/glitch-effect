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
    cout << "Usage: glitch [options] file.txt" << endl;
    cout << "  --help: Display this help message" << endl;
    cout << "  -t <int>: Set maximum delay in miliseconds (default: 100 )" << endl;
    cout << "  -s <int>: Set effect strenght (default: 10, max line deformation strenght)" << endl;
    cout << "  -i <int>: Set effect intensity (default: 20, one line in <n> gets deformed)" << endl;
    cout << "  -ox <int>: Manually set X offset" << endl;
    cout << "  -oy <int>: Manually set Y offset" << endl;
    cout << "  -c <path>: specify config path" << endl;
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

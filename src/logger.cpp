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
    cout << "  --help: Display help message" << endl;
    cout << "  -t <number>: Set maximum delay in miliseconds (default: 100 )" << endl;
    cout << "  -s <number>: Set effect strenght (default: 10, max line deformation strenght)" << endl;
    cout << "  -i <number>: Set effect intensity (default: 20, one line in <n> gets deformed)" << endl;
    cout << "  --autocenter: Enable autocenter" << endl;
    cout << "  -ox <number>: Manually set X offset" << endl;
    cout << "  -oy <number>: Manually set Y offset" << endl;	
}

void Logger::PrintUnsupported(const char *arg)
{
    PrintLog("Unsupported use of an argument: " + string(arg), 0);
	Logger::PrintUsage();
}

void Logger::PrintLog(const string message, const int layer)
{
    cout<<string(layer * 2, ' ')<<GREEN<<"[LOG] "<<RESET<<message<<endl;
}

void Logger::PrintWarn(const string message, const int layer)
{
    cout<<string(layer * 2, ' ')<<YELLOW<<"[WARN] "<<RESET<<message<<endl;
}

void Logger::PrintErr(const string message, const int layer)
{
    cout<<string(layer * 2, ' ')<<RED<<"[ERR] "<<RESET<<message<<endl;
}

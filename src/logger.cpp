#include "Logger.h"

#include <iostream>

void Logger::PrintUsage()
{
	std::cout << "Usage: glitch [options] file.txt" << std::endl;
    std::cout << "  --help: Display help message" << std::endl;
    std::cout << "  -t <number>: Set maximum delay in miliseconds (default: 100 )" << std::endl;
    std::cout << "  -s <number>: Set effect strenght (default: 10, max line deformation strenght)" << std::endl;
    std::cout << "  -i <number>: Set effect intensity (default: 20, one line in <n> gets deformed)" << std::endl;
    std::cout << "  --autocenter: Enable autocenter" << std::endl;
    std::cout << "  -ox <number>: Manually set X offset" << std::endl;
    std::cout << "  -oy <number>: Manually set Y offset" << std::endl;	
}

void Logger::PrintUnsupported(const char *arg)
{
    std::cout << "Unsupported use of an argument: " << arg << std::endl;
	Logger::PrintUsage();
}

void Logger::PrintAutocenterWarn()
{
    std::cout << "You can't manually set offset, autocenter is alredy enabled!" << std::endl;
    PrintUsage();
}

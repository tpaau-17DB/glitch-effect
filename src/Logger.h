#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>

class Logger 
{
	public:
		static void PrintUsage();
		static void PrintUnsupported(const char *arg);
		static void PrintAutocenterWarn();
};

#endif

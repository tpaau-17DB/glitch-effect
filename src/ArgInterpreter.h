#ifndef ARGINTERPRETER_H
#define ARGINTERPRETER_H

#include <unistd.h>
#include <string>

#include "Printer.h"

struct argstruct
{
    std::string ConfigPath = "";
    std::string AsciiPath;

    int OffsetX;
    int OffsetY;

    int ForegroundColor = Printer::NONE;
    int BackgroundColor = Printer::NONE;

    bool ExitRequested = false;
    bool AsciiPathSpecified = false;
    bool ConfigSpecified = false;
    bool HelpRequested = false;
    bool VerbositySet = false;
};

class ArgInterpreter
{
    public:
        static argstruct GetArgs(int argc, char* argv[]);
};

#endif

#ifndef ARGINTERPRETER_H
#define ARGINTERPRETER_H

#include <unistd.h>
#include <string>

#include "Printer.h"

struct argstruct
{
    std::string ConfigPath = "";

    std::string AsciiPath = "";

    bool ForegroundColorSet = false;
    bool BackgroundColorSet = false;
    int ForegroundColor = Printer::NONE;
    int BackgroundColor = Printer::NONE;

    bool VerbositySet = false;

    bool UseChromaticAberration = false;

    bool HelpRequested = false;
    bool ExitRequested = false;
};

class ArgInterpreter
{
    public:
        static argstruct GetArgs(int argc, char* argv[]);
};

#endif

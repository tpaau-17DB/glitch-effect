#ifndef ARGINTERPRETER_H
#define ARGINTERPRETER_H

#include <unistd.h>
#include <string>

#include "Printer.h"

struct argstruct
{
    std::string configPath = "";

    std::string asciiPath = "";

    bool foregroundColorSet = false;
    bool backgroundColorSet = false;
    int foregroundColor = NONE_COLOR;
    int backgroundColor = NONE_COLOR;

    bool verbositySet = false;

    bool useChromaticAberration = false;

    bool helpRequested = false;
    bool exitRequested = false;
};

argstruct getArgs(int argc, char* argv[]);

#endif

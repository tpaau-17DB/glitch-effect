#ifndef ARGINTERPRETER_H
#define ARGINTERPRETER_H

#include <unistd.h>
#include <string>

#include "Printer.h"

struct argstruct
{
    std::string config_path = "../conf/config.conf";
    std::string ascii_path;

    int ox;
    int oy;

    int foreground = Printer::NONE;
    int background = Printer::NONE;

    bool exit = false;
    bool config_specified = false;
    bool help_requested = false;
};

class ArgInterpreter
{
    public:
        static argstruct GetArgs(int argc, char* argv[]);

    private:
        static int strToInt(std::string str);
        static int strToColorID(const std::string colorStr);
};

#endif

#ifndef ARGINTERPRETER_H
#define ARGINTERPRETER_H

#include <unistd.h>
#include <string>

struct argstruct
{
    std::string config_path = "../conf/config.conf";
    std::string ascii_path;

    int ox;
    int oy;

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
};

#endif

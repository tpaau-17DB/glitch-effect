#include <unistd.h>
#include <getopt.h>
#include <iostream>

#include "ArgInterpreter.h"
#include "Logger.h"
#include "Printer.h"

using namespace std;

int ArgInterpreter::strToInt(string str)
{
    int result;

    try
    {
        result = stoi(str);
    }
    catch (const invalid_argument& e)
    {
        result = -1;
        Logger::PrintErr(string("Failed to convert '") + str + string("' to an int. (") + e.what() + string(")"));
    }

    return result;
}

int ArgInterpreter::strToColorID(const string colorStr)
{
    if (colorStr == "NONE")
        return Printer::NONE;
    else if (colorStr == "BLACK")
        return Printer::BLACK;
    else if (colorStr == "RED")
        return Printer::RED;
    else if (colorStr == "GREEN")
        return Printer::GREEN;
    else if (colorStr == "YELLOW")
        return Printer::YELLOW;
    else if (colorStr == "BLUE")
        return Printer::BLUE;
    else if (colorStr == "MAGENTA")
        return Printer::MAGENTA;
    else if (colorStr == "CYAN")
        return Printer::CYAN;
    else if (colorStr == "WHITE")
        return Printer::WHITE;
    Logger::PrintErr("Unknown color. Possible choices: NONE, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE.");
    return -2;
}

void PrintUsage()
{
    cout << "Usage: glitch [options]" << endl;
    cout << "  --help, -h: Display this help message" << endl;
    cout << "  -x <int>: Manually set X offset, disables autocenter" << endl;
    cout << "  -y <int>: Manually set Y offset, disables autocenter" << endl;
    cout << "  -c <path>: specify config file path" << endl;
    cout << "  -i <path>: specify the ascii file to display" << endl;
    cout << "  -v <int>: Set log verbosity" << endl;
    cout << "  --foreground <color>: Set foreground color" << endl;
    cout << "  --background <color>: Set background color" << endl;
}

argstruct ArgInterpreter::GetArgs(int argc, char* argv[])
{
    argstruct args = argstruct();
    
    static struct option long_options[] = 
    {
        {"help", no_argument, nullptr, '1'},
        {"foreground", required_argument, nullptr, '2'},
        {"background", required_argument, nullptr, '3'},
        {0, 0, 0, 0}

    };

    int arg;
    int option_index = 0;
    bool config_specified = false;
    bool ascii_specified = false;
    bool verb_set = false;

    while ((arg = getopt_long(argc, argv, "x:y:v:i:c:h", long_options, &option_index)) != -1)
    {
        switch (arg)
        {
            case 'h':
                args.help_requested = true;
                PrintUsage();
                return args;

            case 'c':
                if (optarg)
                {
                    config_specified = true;
                    args.config_path = optarg;
                    Logger::PrintDebug(string("Config file path set to: '") + optarg + string("'"));
                }

            case 'v':
                if (optarg)
                {
                    if (verb_set) break;
                    verb_set = true;
                    int val = strToInt(optarg);

                    if (val == -1)
                    {
                        args.exit = true;
                        return args;
                    }

                    Logger::SetVerbosity(Logger::LogLevel(val));
                    Logger::PrintDebug(string("Verbosity set to: ") + optarg);
                }
                break;

            case 'i':
                if (optarg)
                {
                    args.ascii_path = optarg;
                    ascii_specified = true;
                    Logger::PrintDebug(string("Ascii file path set to: '") + args.ascii_path + string("'"));
                }
                break;

            case 'x':
                if (optarg)
                {
                    int val = strToInt(optarg);

                    if (val == -1)
                    {
                        args.exit = true;
                        return args;
                    }

                    args.ox = val;
                    Logger::PrintDebug(string("Offset X set to: ") + optarg);
                }
                break;

            case 'y':
                if (optarg)
                {
                    int val = strToInt(optarg);

                    if (val == -1)
                    {
                        args.exit = true;
                        return args;
                    }

                    args.oy = val;
                    Logger::PrintDebug(string("Offset Y set to: ") + optarg);
                }
                break;

            case '1':
                args.help_requested = true;
                PrintUsage();
                return args;

            case '2':
                if (optarg)
                {
                    int val = strToColorID(optarg);

                    if (val == -2)
                    {
                        args.exit = true;
                        return args;
                    }

                    args.foreground = val;
                    Logger::PrintDebug(string("Foreground color set to: ") + optarg);
                }
                break;

            case '3':
                if (optarg)
                {
                    int val = strToColorID(optarg);

                    if (val == -2)
                    {
                        args.exit = true;
                        return args;
                    }

                    args.background = val;
                    Logger::PrintDebug(string("Background color set to: ") + optarg);
                }
                break;
        }
    }

    if (!config_specified)
    {
        Logger::PrintWarn("Config file was not specified!");
    }

    if (!ascii_specified)
    {
        Logger::PrintErr("Ascii file was not specified!");
        args.exit = true;
    }

    return args;
}

#include <unistd.h>
#include <getopt.h>
#include <iostream>

#include "ArgInterpreter.h"
#include "Logger.h"
#include "Utils.h"
#include "METADATA.h"

using namespace std;

const struct option LONG_OPTIONS[] = 
{
    {"help", no_argument, nullptr, '1'},
    {"version", no_argument, nullptr, '2'},
    {"list-colors", no_argument, nullptr, '3'},
    {"foreground", required_argument, nullptr, '4'},
    {"background", required_argument, nullptr, '5'},
    {"chromatic-aberration", no_argument, nullptr, '6'},
    {"nocolor", no_argument, nullptr, '7'},
    {"verbosity", required_argument, nullptr, '8'},
    {0, 0, 0, 0}
};

void printUsage()
{
    cout<<"Usage: glitch [options] [ascii path]\n";
    cout<<"  --help, -h: Display this help message\n";
    cout<<"  --version, -v: Print version\n";
    cout<<"  --list-colors: lists possible colors\n";
    cout<<"  --foreground <color>: Set foreground color\n";
    cout<<"  --background <color>: Set background color\n";
    cout<<"  --chromatic-aberration: Use chromatic aberration\n";
    cout<<"  -c <path> Set the config file\n";
    cout<<"  --verbosity <int>: Set logger verbosity\n";
    cout<<"  --nocolor: enables logger nocolor option\n";
    cout<<"\n";
    cout<<"Runtime options:\n";
    cout<<"  p: pause the program\n";
    cout<<"  q: quit the program\n";
}

void printPossibleColors()
{
    cout << "Possible colors:\n";
    cout << "  * \033[30mblack\033[0m\n";
    cout << "  * \033[31mred\033[0m\n";
    cout << "  * \033[32mgreen\033[0m\n";
    cout << "  * \033[33myellow\033[0m\n";
    cout << "  * \033[34mblue\033[0m\n";
    cout << "  * \033[35mmagenta\033[0m\n";
    cout << "  * \033[36mcyan\033[0m\n";
    cout << "  * \033[37mwhite\033[0m\n";
}

void printVersion()
{
    cout<<PROGRAM_NAME<<" v"<<PROGRAM_VERSION<<"\n";
}

argstruct getArgs(int argc, char* argv[])
{
    argstruct args = argstruct();

    int arg;
    int option_index = 0;
    opterr = 0;
    while ((arg = getopt_long(argc, argv, "hvc:", LONG_OPTIONS, &option_index)) != -1)
    {
        switch (arg)
        {
            case 'h':
                args.helpRequested = true;
                printUsage();
                return args;

            case 'c':
                if (optarg)
                {
                    args.configPath = optarg;
                    PrintDebug(string("Config path set to '")
                        +optarg + string("'."));
                }
                break;

            case 'v':
                printVersion();
                args.exitRequested = true;
                break;

            case '1':
                args.helpRequested = true;
                printUsage();
                return args;

            case '2': // --version
                printVersion();
                args.exitRequested = true;
                break;

            case '3': // --list-colors
                printPossibleColors();
                args.exitRequested = true;
                break;

            case '4': // --foreground
                if (optarg)
                {
                    int val = strToColorID(optarg);

                    if (val == -2)
                    {
                        args.exitRequested = true;
                        PrintErr("Invalid color value!\n");
                        printPossibleColors();
                        return args;
                    }

                    args.foregroundColor = val;
                    args.foregroundColorSet = true;
                    PrintDebug(string("Foreground color set to: ") + optarg);
                }
                break;

            case '5':  // --background
                if (optarg)
                {
                    int val = strToColorID(optarg);

                    if (val == -2)
                    {
                        args.exitRequested = true;
                        PrintErr("Invalid color value!\n");
                        printPossibleColors();
                        return args;
                    }

                    args.backgroundColor = val;
                    args.backgroundColorSet = true;
                    PrintDebug(string("Background color set to: ") + optarg);
                }
                break;


            case '6': // --chromatic-aberration
                args.useChromaticAberration = true;
                PrintDebug("Chromatic aberration enabled.");
                break;

            case '7': // --nocolor
                ToggleLogColor(true);
                PrintDebug("Disabled color in Logger.");
                break;

            case '8':
                if (optarg)
                {
                    int val = strToInt(optarg);

                    if (val == -1)
                    {
                        args.exitRequested = true;
                        return args;
                    }

                    SetLoggerVerbosity(LogLevel(val));
                    args.verbositySet = true;
                    PrintDebug(string("Verbosity set to: ") + to_string(GetLoggerVerbosity()));
                }
                break;

            default:
                PrintErr("Invalid option.");
                printUsage();
                args.exitRequested = true;
                return args;
        }
    }

    if (optind < argc)
    {
        args.asciiPath = argv[optind];
        PrintDebug(string("Ascii file path set to '") + args.asciiPath + string("'."));
    }

    return args;
}

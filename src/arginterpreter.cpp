#include <unistd.h>
#include <getopt.h>
#include <iostream>

#include "ArgInterpreter.h"
#include "Logger.h"
#include "Utils.h"
#include "METADATA.h"

using namespace std;

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
    cout<<GLITCH_NAME<<" v"<<GLITCH_VERSION<<"\n";
}

argstruct ArgInterpreter::GetArgs(int argc, char* argv[])
{
    argstruct args = argstruct();
    static struct option long_options[] = 
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

    int arg;
    int option_index = 0;
    opterr = 0;
    while ((arg = getopt_long(argc, argv, "hvc:", long_options, &option_index)) != -1)
    {
        switch (arg)
        {
            case 'h':
                args.HelpRequested = true;
                printUsage();
                return args;

            case 'c':
                if (optarg)
                {
                    args.ConfigPath = optarg;
                    Logger::PrintDebug(string("Config path set to '")
                            +optarg + string("'."));
                }
                break;

            case 'v':
                printVersion();
                args.ExitRequested = true;
                break;

            case '1':
                args.HelpRequested = true;
                printUsage();
                return args;

            case '2': // --version
                printVersion();
                args.ExitRequested = true;
                break;

            case '3': // --list-colors
                printPossibleColors();
                args.ExitRequested = true;
                break;

            case '4': // --foreground
                if (optarg)
                {
                    int val = Utils::StrToColorID(optarg);

                    if (val == -2)
                    {
                        args.ExitRequested = true;
                        cout<<"Invalid color value!\n";
                        printPossibleColors();
                        return args;
                    }

                    args.ForegroundColor = val;
                    args.ForegroundColorSet = true;
                    Logger::PrintDebug(string("Foreground color set to: ") + optarg);
                }
                break;

            case '5':  // --background
                if (optarg)
                {
                    int val = Utils::StrToColorID(optarg);

                    if (val == -2)
                    {
                        args.ExitRequested = true;
                        cout<<"Invalid color value!\n";
                        printPossibleColors();
                        return args;
                    }

                    args.BackgroundColor = val;
                    args.BackgroundColorSet = true;
                    Logger::PrintDebug(string("Background color set to: ") + optarg);
                }
                break;


            case '6': // --chromatic-aberration
                args.UseChromaticAberration = true;
                Logger::PrintDebug("Chromatic aberration enabled.");
                break;

            case '7': // --nocolor
                Logger::SetNoColor(true);
                Logger::PrintDebug("Disabled color in Logger.");
                break;

            case '8':
                if (optarg)
                {
                    int val = Utils::StrToInt(optarg);

                    if (val == -1)
                    {
                        args.ExitRequested = true;
                        return args;
                    }

                    Logger::SetVerbosity(Logger::LogLevel(val));
                    args.VerbositySet = true;
                    Logger::PrintDebug(string("Verbosity set to: ") + to_string(Logger::GetVerbosity()));
                }
                break;

            default:
                Logger::PrintErr("Invalid option.");
                printUsage();
                args.ExitRequested = true;
                return args;
        }
    }

    if (optind < argc)
    {
        args.AsciiPath = argv[optind];
        Logger::PrintDebug(string("Ascii file path set to '") + args.AsciiPath + string("'."));
    }

    return args;
}

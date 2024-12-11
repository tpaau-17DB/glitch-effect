#include <unistd.h>
#include <getopt.h>
#include <iostream>

#include "ArgInterpreter.h"
#include "Logger.h"
#include "Utils.h"

using namespace std;

void PrintUsage()
{
    cout<<"Usage: glitch [options] [ascii path]\n";
    cout<<"  --help, -h: Display this help message\n";
    cout<<"  -x <int>: Manually set X offset, disables autocenter\n";
    cout<<"  -y <int>: Manually set Y offset, disables autocenter\n";
    cout<<"  -c <path> Set the config file\n";
    cout<<"  -v <int>: Set log verbosity\n";
    cout<<"  --foreground <color>: Set foreground color\n";
    cout<<"  --background <color>: Set background color\n";
    cout<<"  --chromatic-aberration: Use chromatic aberration\n";
    cout<<"  --nocolor: enables nocolor option in Logger\n";
    cout<<"  --list-colors: lists possible colors\n";
}

void PrintPossibleColors()
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

argstruct ArgInterpreter::GetArgs(int argc, char* argv[])
{
    argstruct args = argstruct();
    
    static struct option long_options[] = 
    {
        {"help", no_argument, nullptr, '1'},
        {"foreground", required_argument, nullptr, '2'},
        {"background", required_argument, nullptr, '3'},
        {"chromatic-aberration", no_argument, nullptr, '4'},
        {"nocolor", no_argument, nullptr, '5'},
        {"list-colors", no_argument, nullptr, '6'},
        {0, 0, 0, 0}

    };

    int arg;
    int option_index = 0;

    while ((arg = getopt_long(argc, argv, "c:v:x:y:h", long_options, &option_index)) != -1)
    {
        switch (arg)
        {
            case 'h':
                args.HelpRequested = true;
                PrintUsage();
                return args;

            case 'c':
                if (optarg)
                {
                    args.ConfigPath = optarg;
                    args.ConfigSpecified = true;
                    Logger::PrintDebug(string("Config path set to '") + optarg + string("'."));
                }
                break;

            case 'x':
                if (optarg)
                {
                    int val = Utils::StrToInt(optarg);

                    if (val == -1)
                    {
                        args.ExitRequested = true;
                        return args;
                    }

                    args.OffsetX = val;
                    Logger::PrintDebug(string("Offset X set to: ") + optarg);
                }
                break;

            case 'y':
                if (optarg)
                {
                    int val = Utils::StrToInt(optarg);

                    if (val == -1)
                    {
                        args.ExitRequested = true;
                        return args;
                    }

                    args.OffsetY = val;
                    Logger::PrintDebug(string("Offset Y set to: ") + optarg);
                }
                break;

            case '1':
                args.HelpRequested = true;
                PrintUsage();
                return args;

            case '2': // --foreground
                if (optarg)
                {
                    int val = Utils::StrToColorID(optarg);

                    if (val == -2)
                    {
                        args.ExitRequested = true;
                        cout<<"Invalid color value!\n";
                        PrintPossibleColors();
                        return args;
                    }

                    args.ForegroundColor = val;
                    args.ForegroundColorSet = true;
                    Logger::PrintDebug(string("Foreground color set to: ") + optarg);
                }
                break;

            case '3':  // --background
                if (optarg)
                {
                    int val = Utils::StrToColorID(optarg);

                    if (val == -2)
                    {
                        args.ExitRequested = true;
                        cout<<"Invalid color value!\n";
                        PrintPossibleColors();
                        return args;
                    }

                    args.BackgroundColor = val;
                    args.BackgroundColorSet = true;
                    Logger::PrintDebug(string("Background color set to: ") + optarg);
                }
                break;

                case 'v':
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

            case '4': // --chromatic-aberration
                args.UseChromaticAberration = true;
                Logger::PrintDebug("Chromatic aberration enabled.");
                break;

            case '5': // --nocolor
                Logger::SetNoColor(true);
                Logger::PrintDebug("Disabled color in Logger.");
                break;

            case '6':
                PrintPossibleColors();
                args.ExitRequested = true;
                break;
        }
    }

    if (optind < argc)
    {
        args.AsciiPath = argv[optind];
        args.AsciiPathSpecified = true;
        Logger::PrintDebug(string("Ascii file path set to '") + args.AsciiPath + string("'."));
    }

    return args;
}

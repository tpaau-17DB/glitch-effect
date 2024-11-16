#include <unistd.h>
#include <getopt.h>
#include <iostream>

#include "ArgInterpreter.h"
#include "Logger.h"
#include "Utils.h"

using namespace std;

void PrintUsage()
{
    cout<<"Usage: glitch [options] [ascii path]"<<endl;
    cout<<"  --help, -h: Display this help message"<<endl;
    cout<<"  -x <int>: Manually set X offset, disables autocenter"<<endl;
    cout<<"  -y <int>: Manually set Y offset, disables autocenter"<<endl;
    cout<<"  -c <path> Set the config file"<<endl;
    cout<<"  -v <int>: Set log verbosity"<<endl;
    cout<<"  --foreground <color>: Set foreground color"<<endl;
    cout<<"  --background <color>: Set background color"<<endl;
    cout<<"  --nocolor: enables nocolor option in Logger"<<endl;
}

argstruct ArgInterpreter::GetArgs(int argc, char* argv[])
{
    argstruct args = argstruct();
    
    static struct option long_options[] = 
    {
        {"help", no_argument, nullptr, '1'},
        {"foreground", required_argument, nullptr, '2'},
        {"background", required_argument, nullptr, '3'},
        {"nocolor", no_argument, nullptr, '4'},
        {0, 0, 0, 0}

    };

    int arg;
    int option_index = 0;
    bool ascii_specified = false;

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

            case '2':
                if (optarg)
                {
                    int val = Utils::StrToColorID(optarg);

                    if (val == -2)
                    {
                        args.ExitRequested = true;
                        return args;
                    }

                    args.ForegroundColor = val;
                    Logger::PrintDebug(string("Foreground color set to: ") + optarg);
                }
                break;

            case '3':
                if (optarg)
                {
                    int val = Utils::StrToColorID(optarg);

                    if (val == -2)
                    {
                        args.ExitRequested = true;
                        return args;
                    }

                    args.BackgroundColor = val;
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

            case '4':
                Logger::SetNoColor(true);
                Logger::PrintDebug("Disabled color in Logger.");
                break;
        }
    }

    if (optind < argc)
    {
        args.AsciiPath = argv[optind];
        ascii_specified = true;
        Logger::PrintDebug(string("Ascii file path set to '") + args.AsciiPath + string("'."));
    }

    if (!ascii_specified)
    {
        Logger::PrintErr("Ascii file was not specified!");
        args.ExitRequested = true;
    }

    return args;
}

#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <csignal>
#include <ncurses.h>

#include "Logger.h"
#include "FileLoader.h"
#include "ArgInterpreter.h"
#include "AsciiBuffer.h"
#include "Printer.h"
#include "ConfigLoader.h"
#include "Utils.h"

using namespace std;


// when this variable is `true`, the program should exit immediately
bool exitRequested = false;



void handleSignal(int signal) 
{
    if (signal == SIGINT) 
    {
        Logger::PrintDebug("Exit requested by user.");
        exitRequested = true;
    }
}


int main(int argc, char *argv[])
{
    signal(SIGINT, handleSignal);
    srand(time(nullptr));

    int sleeptimeMS = 40;
    bool useChromaticAberration = false;

    // get command line arguments and handle them
    argstruct args = ArgInterpreter::GetArgs(argc, argv);
    if (args.HelpRequested)
    {
        Logger::PrintDebug("Help requested by user.");
        Logger::ReleaseLogBuffer();
        return 0;
    }
    else if (args.ExitRequested)
    {
        Logger::PrintDebug("ArgInterpreter requested exit.");
        Logger::ReleaseLogBuffer();
        return 1;
    }
    else
    {
        useChromaticAberration = args.UseChromaticAberration;
        Printer::SetDefaultColors(Printer::Color(args.ForegroundColor), Printer::Color(args.BackgroundColor));
    }

    // obtain the path of the config file
    string confPath;
    if (args.ConfigSpecified)
    {
        confPath = args.ConfigPath;
    }
    else
    {
        Logger::PrintDebug("Config file not specified.");
        confPath = FileLoader::LookForConfigFiles();
    }

    // load the config file
    ConfigLoader::GlobalConfig config; 
    vector<ConfigLoader::pass> passes;
    if (confPath != "" && confPath != "none")
    {
       
        passes = ConfigLoader::GetPassesFromJSON(confPath);
        config = ConfigLoader::GetGlobalConfig(confPath);
    }

    // check if passes were loaded correctly and if not, proceed with default pass 
    if (passes.size() == 0)
    {
        Logger::PrintLog("Falling back to default pass.");

        ConfigLoader::pass pass = ConfigLoader::pass();
        pass.Type = ConfigLoader::HorizontalDistort;
        pass.Intensity = 10;
        pass.Strength = 7;
        passes.push_back(pass);

        pass.Intensity = 25;
        pass.Strength = 1;
        passes.push_back(pass);
    }

    // apply the config file if loaded correctly
    if (config.LoadedCorrectly)
    {
        if (!args.VerbositySet)
        {
            Logger::SetVerbosity((int)config.LoggerVerbosity);
        }
        sleeptimeMS = config.SleeptimeMS;
        
        // Set useChromaticAberration only if it was not set previously using cli args.
        if (!useChromaticAberration)
            useChromaticAberration = config.UseChromaticAberration;

        if (!args.ForegroundColorSet)
        {
            Printer::SetDefaultForegroundColor(Printer::Color(Utils::StrToColorID(config.foregroundColorName)));
        }

        if (!args.BackgroundColorSet)
        {
            Printer::SetDefaultBackgroundColor(Printer::Color(Utils::StrToColorID(config.backgroundColorName)));
        }
    }
    else
    {
        Logger::PrintLog("Using default config.");
    }

    string asciiPath;
    if (args.AsciiPathSpecified)
    {
        asciiPath = args.AsciiPath; 
    }
    else if (config.DefaultAsciiPath != "///notspecified///" && config.LoadedCorrectly)
    {
        Logger::PrintDebug("Using ascii file path specified in '" + confPath + "'.");
        asciiPath = config.DefaultAsciiPath;
    }
    else
    {
        Logger::PrintErr("Ascii file path was not specified!");
        Logger::ReleaseLogBuffer();
        return 1;
    }

    const vector<string> lines = FileLoader::GetLines(asciiPath);
    if (lines.size() == 0)
    {
        Logger::PrintDebug("Nothing to display. Quitting...");
        Logger::ReleaseLogBuffer();
        return 1;
    }
   
    // initialize printer and buffer
    Printer::Init(sleeptimeMS, args.OffsetX, args.OffsetY);
    AsciiBuffer buffer = AsciiBuffer(lines);

    bool isPaused = false;
    // last key pressed in the loop
    char lastKey;
    int ch;
    while (!exitRequested)
    {
        ch = getch();
        switch (ch)
        {
            case 'q':
                Printer::Stop();
                Logger::ReleaseLogBuffer();
                return 0;

            case 'p':
                if (lastKey != 'p')
                    isPaused = !isPaused;
                lastKey = 'p';
                break;

            default:
                lastKey = '\0';
                break;
        }

        if (isPaused)
        {
            usleep(1000);
            continue;
        }

        // apply the passes and check if there were any errors
        if (buffer.ApplyPasses(passes) != 0)
        {
            Logger::PrintErr("Errors occurred while applying passes. See errors above.");
            break;
        }

        // print the distorted image
        Printer::Print(buffer, useChromaticAberration);
        buffer.ResetDistorted();

        refresh();
        usleep(1000 * sleeptimeMS);
    }

    Printer::Stop();
    Logger::ReleaseLogBuffer();
    return 0;
}

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

using namespace std;

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
    Logger::SetUseLogAccumulation(true);
    srand(time(nullptr));

    int sleeptimeMS = 40;

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

    ConfigLoader::GlobalConfig globalConfig; 
    vector<ConfigLoader::pass> passes;
    if (args.ConfigSpecified)
    {
        passes = ConfigLoader::GetPassesFromJSON(args.ConfigPath);
        globalConfig = ConfigLoader::GetGlobalConfig(args.ConfigPath);
    }
    else
    {
        Logger::PrintWarn("Config file not specified.");
        string confPath = FileLoader::LookForConfigFiles();

        if (confPath != "none")
        {
            passes = ConfigLoader::GetPassesFromJSON(confPath);
            globalConfig = ConfigLoader::GetGlobalConfig(confPath);
        }
    }

    if (passes.size() == 0)
    {
        Logger::PrintLog("Falling back to default pass.");

        ConfigLoader::pass pass = ConfigLoader::pass();
        pass.Type = ConfigLoader::VerticalDistort;
        pass.Intensity = 35;
        pass.Strength = 7;
        passes.push_back(pass);
    }

    if (globalConfig.LoadedCorrectly)
    {
        if (!args.VerbositySet)
        {
            Logger::SetVerbosity((int)globalConfig.LoggerVerbosity);
            Logger::PrintDebug("Verbosity set to: " + to_string(globalConfig.LoggerVerbosity));
        }
        sleeptimeMS = globalConfig.SleeptimeMS;
    }
    else
    {
        Logger::PrintWarn("Using default config.");
    }

    const vector<string> lines = FileLoader::GetLines(args.AsciiPath);
    if (lines.size() == 0)
    {
        Logger::PrintDebug("Nothing to display. Quitting...");
        Logger::ReleaseLogBuffer();
        return 1;
    }
    
    Printer::Init(sleeptimeMS, args.OffsetX, args.OffsetY);
    Printer::SetColors(Printer::Color(args.ForegroundColor), Printer::Color(args.BackgroundColor));

    AsciiBuffer buffer = AsciiBuffer(lines);
  
    int exitCode;
    while (!exitRequested)
    {
        exitCode = buffer.ApplyPasses(passes);

        if (exitCode != 0)
        {
            Logger::PrintErr("Errors occured while applying passes. See errors above.");
        }

        Printer::Print(buffer, false);
        buffer.ResetDistorted();
        
	int ch = getch();
	if (ch == 'q')
        {
            Logger::PrintDebug("Exit requested by user.");
            break;
        }

        refresh();

        usleep(1000 * sleeptimeMS);
    }
    
    Printer::Stop();
    Logger::ReleaseLogBuffer();
    return 0;
}

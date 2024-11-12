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

    argstruct args;

    int sleeptime_ms = 40;

    args = ArgInterpreter::GetArgs(argc, argv);
    if (args.help_requested)
    {
        Logger::PrintDebug("Help requested by user.");
        Logger::ReleaseLogBuffer();
        return 0;
    }
    else if (args.exit)
    {
        Logger::PrintDebug("ArgInterpreter requested exit.");
        Logger::ReleaseLogBuffer();
        return 1;
    }

    vector<ConfigLoader::pass> passes;

    bool configOK = false;
    if (args.config_specified)
    {
        passes = ConfigLoader::GetPassesFromJSON(args.config_path);
        configOK = true;
    }
    else
    {
        Logger::PrintDebug("Config file not specified.");
        string confPath = FileLoader::LookForConfigFiles();

        if (confPath != "none")
        {
            passes = ConfigLoader::GetPassesFromJSON(confPath);
            configOK = true;
        }
        else
        {
            configOK = false;
        }
    }

    if (passes.size() == 0 || !configOK)
    {
        if (configOK)
        {
            Logger::PrintErr("Errors occured while loading config file. Proceeding with default config.");
        }
        
        ConfigLoader::pass pass = ConfigLoader::pass();
        pass.PT = ConfigLoader::VerticalDistort;
        pass.PP.Intensity = 35;
        pass.PP.Strength = 7;
        passes.push_back(pass);
    }
    else
    {
        Logger::PrintDebug(to_string(passes.size()) + " passes loaded.");
    }

    const vector<string> lines = FileLoader::GetLines(args.ascii_path);
    if (lines.size() == 0)
    {
        Logger::PrintDebug("Nothing to display. Quitting...");
        Logger::ReleaseLogBuffer();
        return 1;
    }

    
    Printer::init(sleeptime_ms, args.ox, args.oy);
    Printer::SetColors(Printer::Color(args.foreground), Printer::Color(args.background));

    AsciiBuffer buffer = AsciiBuffer(lines);
   
    while (!exitRequested)
    {
        buffer.ApplyPasses(passes);
        Printer::print(buffer, false);
        buffer.ResetDistorted();
        
	int ch = getch();
	if (ch == 'q')
        {
            Logger::PrintDebug("Exit requested by user.");
            break;
        }

        refresh();

        usleep(1000 * sleeptime_ms);
    }
    
    Printer::stop();
    Logger::ReleaseLogBuffer();
    return 0;
}

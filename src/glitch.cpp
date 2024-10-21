#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <csignal>
#include <ncurses.h>

#include "Logger.h"
#include "FileLoader.h"
#include "ConfigLoader.h"
#include "ArgInterpreter.h"
#include "AsciiBuffer.h"
#include "Printer.h"

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

    args.ascii_path = "";
    args.config_path = "";

    int sleeptime_ms = 40;
    int effectStrength = 8;
    int effectIntensity = 35;

    args = ArgInterpreter::GetArgs(argc, argv);
    if (args.help_requested)
    {
        Logger::PrintDebug("Help requested by user. Exiting...");
        return 0;
    }
    else if (args.exit)
    {
        Logger::PrintDebug("Told to exit by the arginterpreter. Exiting now...");
        return 0;
    }

    int conf_exit_code = 1;
    map<string, int> config;

    if (args.config_specified)
    {
        config = ConfigLoader::LoadConf(args.config_path);
        conf_exit_code = ConfigLoader::GetParamFromConf(&config, "exit-code");
    }
    else
    {
        string path = FileLoader::CheckForConfigFiles();

        if (path != "none")
        {
            config = ConfigLoader::LoadConf(args.config_path);
            conf_exit_code = ConfigLoader::GetParamFromConf(&config, "exit-code");
        }
    }

    int output_tmp;

    if (conf_exit_code == 0)
    {
	Logger::PrintDebug("Config loaded succesfully.");

    	output_tmp = ConfigLoader::GetParamFromConf(&config, "strength");
    	if (output_tmp != -1)
    	    effectStrength = output_tmp;

    	output_tmp = ConfigLoader::GetParamFromConf(&config, "intensity");
    	if (output_tmp != -1)
    	    effectIntensity = output_tmp;

	output_tmp = ConfigLoader::GetParamFromConf(&config, "sleeptime");
    	if (output_tmp != -1)
    	    sleeptime_ms = output_tmp;
    }

    const vector<string> lines = FileLoader::GetLines(args.ascii_path);
    if (lines.size() == 0)
    {
        Logger::PrintWarn("Nothing to display. Quitting...");
        return 0;
    }
    
    Printer::init(sleeptime_ms, args.ox, args.oy);
    Printer::SetColors(Printer::Color(args.foreground), Printer::Color(args.background));

    AsciiBuffer buffer = AsciiBuffer(lines);
    
    while (!exitRequested)
    {
        buffer.VerticalDistort(effectIntensity, effectStrength);
        Printer::print(buffer, effectStrength);
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

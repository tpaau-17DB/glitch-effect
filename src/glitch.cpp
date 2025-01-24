#include <fcntl.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <csignal>
#include <ncurses.h>

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
        PrintDebug("SIGINT recieved.");
        exitRequested = true;
    }
}


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.UTF-8");

    signal(SIGINT, handleSignal);
    srand(time(nullptr));

    int sleeptimeMS = 40;
    bool useChromaticAberration = false;

    // get command line arguments and handle them
    argstruct args = ArgInterpreter::GetArgs(argc, argv);
    if (args.HelpRequested)
    {
        PrintDebug("Help requested by user.");
        exit(EXIT_SUCCESS);
    }
    else if (args.ExitRequested)
    {
        PrintDebug("ArgInterpreter requested exit.");
        exit(EXIT_SUCCESS);
    }
    else
    {
        useChromaticAberration = args.UseChromaticAberration;
        Printer::SetDefaultColors(
                Printer::Color(args.ForegroundColor),
                Printer::Color(args.BackgroundColor));
    }

    // obtain the path of the config file
    string confPath;
    if (args.ConfigPath != "")
    {
        PrintDebug("Using config file '" + args.ConfigPath + "'.");
        confPath = args.ConfigPath;
    }
    else
    {
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
        PrintLog("Falling back to default pass.");

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
            SetLoggerVerbosity((int)config.LoggerVerbosity);
        }
        sleeptimeMS = config.SleeptimeMS;
        
        // Set this only if it was not set previously using cli interface 
        if (!useChromaticAberration)
            useChromaticAberration = config.UseChromaticAberration;

        if (!args.ForegroundColorSet)
        {
            Printer::SetDefaultForegroundColor(
                    Printer::Color(
                        Utils::StrToColorID(config.foregroundColorName)));
        }

        if (!args.BackgroundColorSet)
        {
            Printer::SetDefaultBackgroundColor(
                    Printer::Color(
                        Utils::StrToColorID(config.backgroundColorName)));
        }
    }
    else
    {
        PrintLog("Using default config.");
    }

    bool inputPiped = false;
    string asciiPath;
    vector<string> lines;
    if (!isatty(STDIN_FILENO))
    {
        string line;
        while (getline(cin, line)) 
        {
            lines.push_back(line);
        }
        inputPiped = true;
    }
    else if (args.AsciiPath != "")
    {
        asciiPath = args.AsciiPath; 
    }
    else if (config.DefaultAsciiPath != "" && config.LoadedCorrectly)
    {
        PrintDebug("Using ascii file path specified in '" + confPath + "'.");
        asciiPath = config.DefaultAsciiPath;
    }
    else
    {
        PrintCrit("No input to display.");
        exit(EXIT_FAILURE);
    }

    int fd;
    if (!inputPiped)
        lines = FileLoader::GetLines(asciiPath);
    else
    {
        // set stdin to /dev/tty so program will receive user input,
        // even when some input was piped
        fd = open("/dev/tty", O_RDONLY);
        if (fd == -1) 
        {
            PrintErr("Failed to open /dev/tty, user input might not work as exected!");
        }
        else if (close(0) == -1) 
        {
            PrintErr("Failed to close stdin, user input might not work as expected!");
            close(fd);
        } 
        else if (dup2(fd, 0) == -1) 
        {
            PrintErr("Failed to redirect stdin to /dev/tty, user input might not work as expected!");
            close(fd);
        }
    }

    if (lines.size() == 0)
    {
        PrintCrit("Vector empty, nothing to display.");
        exit(EXIT_FAILURE);
    }
   
    // initialize printer and buffer
    Printer::Init(sleeptimeMS);
    AsciiBuffer buffer = AsciiBuffer(lines);

    bool isPaused = false;
    // last key pressed in the loop
    char lastKey;
    int ch;
    string line;
    while (!exitRequested)
    {
        ch = getch();

        switch (ch)
        {
            case 'q':
                PrintDebug("Key 'q' pressed, quitting.");
                clog.flush();
                Printer::Stop();
                exit(EXIT_SUCCESS);

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
            PrintErr("Errors occurred while applying passes.");
            break;
        }

        // print the distorted image
        Printer::Print(buffer, useChromaticAberration);
        buffer.ResetDistorted();

        refresh();
        usleep(1000 * sleeptimeMS);
    }

    close(fd);
    Printer::Stop();
    exit(EXIT_SUCCESS);
}

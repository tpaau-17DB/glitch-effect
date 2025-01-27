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
    argstruct args = getArgs(argc, argv);
    if (args.helpRequested)
    {
        PrintDebug("Help requested by user.");
        exit(EXIT_SUCCESS);
    }
    else if (args.exitRequested)
    {
        PrintDebug("ArgInterpreter requested exit.");
        exit(EXIT_SUCCESS);
    }
    else
    {
        useChromaticAberration = args.useChromaticAberration;
        setForegroundColor(Color(args.foregroundColor));
        setBackgroundColor(Color(args.foregroundColor));
    }

    // obtain the path of the config file
    string confPath;
    if (args.configPath != "")
    {
        confPath = args.configPath;
    }
    else
    {
        confPath = lookForConfigFile();
    }

    // load the config file
    GlobalConfig config; 
    vector<Pass> passes;
    if (confPath != "" && confPath != "none")
    {
        PrintDebug("Using config file '" + args.configPath + "'.");
        passes = getPassesFromJSON(confPath);
        config = getGlobalConfig(confPath);
    }

    // check if passes were loaded correctly and if not, proceed with default pass 
    if (passes.size() == 0)
    {
        PrintLog("Falling back to default pass.");

        Pass pass = Pass();
        pass.type = HORIZONTAL_DISTORT;
        pass.intensity = 10;
        pass.strength = 7;
        passes.push_back(pass);

        pass.intensity = 25;
        pass.strength = 1;
        passes.push_back(pass);
    }

    // apply the config file if loaded correctly
    if (config.loadedCorrectly)
    {
        if (!args.verbositySet)
        {
            SetLoggerVerbosity((int)config.loggerVerbosity);
        }
        sleeptimeMS = config.sleeptimeMS;
        
        // Set this only if it was not set previously using cli interface 
        if (!useChromaticAberration)
            useChromaticAberration = config.chromaticAberrationEnabled;

        if (!args.foregroundColorSet)
        {
            setForegroundColor(
                Color(strToColorID(config.foregroundColorStr)));
        }

        if (!args.backgroundColorSet)
        {
            setBackgroundColor(
                Color(strToColorID(config.backgroundColorStr)));
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
    else if (args.asciiPath != "")
    {
        asciiPath = args.asciiPath; 
    }
    else if (config.defaultAsciiPath != "" && config.loadedCorrectly)
    {
        PrintDebug("Using ascii file path specified in '" + confPath + "'.");
        asciiPath = config.defaultAsciiPath;
    }
    else
    {
        PrintCrit("No input to display.");
        exit(EXIT_FAILURE);
    }

    int fd;
    if (!inputPiped)
        lines = getLines(asciiPath);
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
    initPrinter(sleeptimeMS);
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
                stopPrinter();
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
        if (buffer.applyPasses(passes) != 0)
        {
            PrintErr("Errors occurred while applying passes.");
            break;
        }

        // print the distorted image
        printDistortedText(buffer, useChromaticAberration);
        buffer.resetDistorted();

        refresh();
        usleep(1000 * sleeptimeMS);
    }

    close(fd);
    stopPrinter();
    exit(EXIT_SUCCESS);
}

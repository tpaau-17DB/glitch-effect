#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "FileLoader.h"
#include "Logger.h"

using namespace std;
using namespace filesystem;

const string FileLoader::configPaths[] = 
{
    "~/.config/glitch-effect/config.conf",
    "~/.config/glitch-effect/config",
    "~/.config/glitch/config.conf",
    "~/.config/glitch/config"
};

vector<string> FileLoader::GetLines(const string &path)
{
    vector<string> lines;
    ifstream file(path);

    if (!file.good())
    {
        Logger::PrintErr("File error! File path: '" + path + "'");
    }

    if (file.is_open())
    {
        string line;

        while (getline(file, line))
        {
            lines.push_back(line);
        }

        file.close();
    }
    else
    {
	Logger::PrintErr("Unable to open file: '" + path + "'");
    }

    return lines;
}

bool FileLoader::CheckIfFileExists(const string &path)
{
    string homeDir = getenv("HOME");
    string fullPath = homeDir + "/" + path.substr(2);

    return filesystem::exists(fullPath);
}

string FileLoader::CheckForConfigFiles()
{
    Logger::PrintDebug("Checking for config file in common directories.");

    for (const string &path : configPaths)
    {
        if (CheckIfFileExists(path))
        {
            Logger::PrintDebug(string("Config file found at: '") + path + string("'."));
            return path;
        }
    }

    Logger::PrintLog("No config files found.");
    return "none";
}

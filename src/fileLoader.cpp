#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "FileLoader.h"
#include "Logger.h"

using namespace std;
using namespace filesystem;


const string configPaths[] = 
{
    FileLoader::GetXDG_CONFIG_HOME() + "/config.jsonc",
    FileLoader::GetXDG_CONFIG_HOME() + "/config.json",
    FileLoader::GetXDG_CONFIG_HOME() + "/config",
    "~/.config/glitch-effect/config.jsonc",
    "~/.config/glitch-effect/config.json",
    "~/.config/glitch-effect/config",
    "config.jsonc",
    "config.json",
    "config",
};


string FileLoader::GetXDG_CONFIG_HOME()
{
    const char* xdgConfigHome = getenv("XDG_CONFIG_HOME");
    if (xdgConfigHome != nullptr) 
    {
        return string(xdgConfigHome);
    }
    return "";
}


string FileLoader::ExpandPath(const string& path)
{
    string fullPath = path;

    if (!fullPath.empty() && fullPath[0] == '~')
    {
        string homeDir = getenv("HOME");
        return homeDir + fullPath.substr(1);
    } 
    else 
    {
        return fullPath;
    }
}

vector<string> FileLoader::GetLines(string &path)
{
    path = ExpandPath(path);

    vector<string> lines;
    ifstream file(path);

    if (!file.good())  // u good?
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
    string fullPath = ExpandPath(path); 
    return filesystem::exists(fullPath);
}

string FileLoader::LookForConfigFiles()
{
    Logger::PrintDebug("Looking for config files in common directories.");

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

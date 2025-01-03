#include <exception>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

#include <nlohmann/json.hpp>

#include "FileLoader.h"
#include "Logger.h"
#include "ConfigLoader.h"
#include "Utils.h"
#include "Printer.h"

using namespace std;

using json = nlohmann::json;

vector<ConfigLoader::pass> ConfigLoader::GetPassesFromJSON(string& path)
{
    vector<ConfigLoader::pass> passes = vector<ConfigLoader::pass>();

    path = FileLoader::ExpandPath(path);

    Logger::PrintDebug("Loading a JSON file '" + path + "'.");

    try
    {
        ifstream f(path);

        if (!f.is_open()) 
        {
            Logger::PrintErr("Could not open the file!");
            return passes;
        }

        stringstream buffer;
        buffer << f.rdbuf();
        string content = buffer.str();

        content = Utils::RemoveComments(content);

        json data = json::parse(content);

        for (auto& [key, value] : data.items()) 
        {
            if (key == "global_config") continue;

            ConfigLoader::pass pass;
            if (value.contains("name"))
                pass.Type = Utils::GetPassTypeFromName(value["name"]);
            
            if (value.contains("strength"))
                pass.Strength = value["strength"];

            if (value.contains("intensity"))
                pass.Intensity = value["intensity"];
            
            if (value.contains("invert"))
                pass.ReverseColor = value["invert"];

            passes.push_back(pass);
        }

        Logger::PrintDebug(to_string(passes.size()) + string(" passes loaded."));
    }
    catch(exception& e)
    {
        Logger::PrintErr("Error occurred while loading a file: '" + path + "': " + e.what());
        return passes;
    }

    return passes;
}

ConfigLoader::GlobalConfig ConfigLoader::GetGlobalConfig(string& path)
{
    ConfigLoader::GlobalConfig globalConfig;
    string tmp;

    Logger::PrintDebug("Loading a JSON file '" + path + "'.");

    try
    {
        ifstream f(path);
        if (!f.is_open()) 
        {
            Logger::PrintErr("Could not open the file!");
            return globalConfig;
        }

        stringstream buffer;
        buffer << f.rdbuf();
        string content = buffer.str();

        content = Utils::RemoveComments(content);

        json data = json::parse(content);

        if (data.contains("global_config"))
        {
            json globalData = data["global_config"];

            if (globalData.contains("sleeptime_ms"))
                globalConfig.SleeptimeMS = globalData["sleeptime_ms"];

            if (globalData.contains("logger_verbosity"))
                globalConfig.LoggerVerbosity = globalData["logger_verbosity"];

            if (globalData.contains("default_ascii_path"))
                globalConfig.DefaultAsciiPath = globalData["default_ascii_path"];

            if (globalData.contains("chromatic_aberration"))
                globalConfig.UseChromaticAberration = globalData["chromatic_aberration"];

            if (globalData.contains("foreground_color"))
            {
                tmp = globalData["foreground_color"];
                if (Printer::IsValidColorID(Utils::StrToColorID(tmp)))
                    globalConfig.foregroundColorName = tmp;
                else
                    Logger::PrintErr("Invalid color ID: " + tmp);
            }

            if (globalData.contains("background_color"))
            {
                tmp = globalData["background_color"];
                if (Printer::IsValidColorID(Utils::StrToColorID(tmp)))
                    globalConfig.backgroundColorName = tmp;
                else
                    Logger::PrintErr("Invalid color ID: " + tmp);
            }
        }
        else
        {
            Logger::PrintLog("No 'global_config' object in JSON file '" + path + "'.");        
        }

        Logger::PrintDebug("Finished loading config.");
    }
    catch(const exception& e)
    {
        Logger::PrintErr("Error occurred while loading a file: '" + path + "': " + e.what());
        return globalConfig;
    }

    globalConfig.LoadedCorrectly = true;
    return globalConfig;
}

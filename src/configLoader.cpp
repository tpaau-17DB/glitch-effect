#include <exception>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "Logger.h"
#include "ConfigLoader.h"

using namespace std;

using json = nlohmann::json;

unsigned long hashString(const string& str) 
{
    unsigned int hash = 5381;
    for (char c : str) 
    {
        hash = (hash * 33) ^ c;
    }
    return hash;
}

ConfigLoader::PassType getPassTypeFromName(const string& name)
{
    ConfigLoader::PassType type = ConfigLoader::Undefined;

    unsigned long hash = hashString(name);

    //Logger::PrintDebug(to_string(hash));

    switch(hash)
    {
        case 3059108212:  // 'vertical distort'
            type = ConfigLoader::VerticalDistort;
            break;

        default:
            Logger::PrintErr("Unknown pass type: '" + name + "'.");
            break;
    }

    return type;
}

vector<ConfigLoader::pass> ConfigLoader::GetPassesFromJSON(const string& path)
{
    vector<ConfigLoader::pass> passes = vector<ConfigLoader::pass>();

    Logger::PrintDebug("Loading a JSON file '" + path + "'.");

    try
    {
        ifstream f(path);

        if (!f.is_open()) 
        {
            Logger::PrintErr("Could not open the file!");
            return passes;
        }

        json data = json::parse(f);

        for (auto& [key, value] : data.items()) 
        {
            if (key == "global_config") continue;

            ConfigLoader::pass pass;
            pass.PT = getPassTypeFromName(value["name"]);
            pass.PP.Strength = value["strength"];
            pass.PP.Intensity = value["intensity"];
            pass.PP.RevCol = value["invert"];
            passes.push_back(pass);
        }

        Logger::PrintDebug("Finished loading passes.");
    }
    catch(exception& e)
    {
        Logger::PrintErr("Error occured while loading a file: '" + path + "': " + e.what());
        return passes;
    }

    return passes;
}

ConfigLoader::GlobalConfig ConfigLoader::GetGlobalConfig(const string& path)
{
    ConfigLoader::GlobalConfig globalConfig;

    Logger::PrintDebug("Loading a JSON file '" + path + "'.");

    try
    {
        ifstream f(path);
        if (!f.is_open()) 
        {
            Logger::PrintErr("Could not open the file!");
            return globalConfig;
        }
        json data = json::parse(f);

        if (data.contains("global_config"))
        {
            json globalData = data["global_config"];
            globalConfig.SleeptimeMS = globalData["sleeptime_ms"];
            globalConfig.LoggerVerbosity = globalData["logger_verbosity"];
        }
        else
        {
            Logger::PrintLog("No 'global_config' object in JSON file '" + path + "'.");        
        }

        Logger::PrintDebug("Finished loading config.");
    }
    catch(exception& e)
    {
        Logger::PrintErr("Error occured while loading a file: '" + path + "': " + e.what());
        return globalConfig;
    }

    globalConfig.loadedCorrectly = true;
    return globalConfig;
}
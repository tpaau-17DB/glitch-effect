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
            string name = value["name"];
            int strength = value["strength"];
            int intensity = value["intensity"];
            bool invert = value["invert"];
            
            ConfigLoader::pass pass;
            pass.PT = getPassTypeFromName(name);
            pass.PP.Strength = strength;
            pass.PP.Intensity = intensity;
            pass.PP.RevCol = invert;

            passes.push_back(pass);
        }

        Logger::PrintDebug("Finished processing JSON file.");
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


        int sleeptime_ms = data["sleeptime_ms"];
        int logger_verbosity = data["logger_verbosity"];

        globalConfig.LoggerVerbosity = logger_verbosity;
        globalConfig.SleeptimeMS = sleeptime_ms;

        Logger::PrintDebug("Finished processing JSON file.");
    }
    catch(exception& e)
    {
        Logger::PrintErr("Error occured while loading a file: '" + path + "': " + e.what());
        return globalConfig;
    }

    globalConfig.loadedCorrectly = true;
    return globalConfig;
}

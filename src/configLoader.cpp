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

using namespace std;

using json = nlohmann::json;


ConfigLoader::PassType getPassTypeFromName(const string& name)
{
    ConfigLoader::PassType type = ConfigLoader::Undefined;

    unsigned long hash = Utils::HashString(name);

    switch(hash)
    {
        case 3091456344:  // 'horizontal distort'
            type = ConfigLoader::HorizontalDistort;
            break;

        case 2811326735:  // 'discard'
            type = ConfigLoader::Discard;
            break;

        default:
            Logger::PrintErr("Unknown pass type: '" + name + "'.");
            break;
    }

    return type;
}

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

        //std::cout<<data.dump()<<endl;
        for (auto& [key, value] : data.items()) 
        {
            if (key == "global_config") continue;

            ConfigLoader::pass pass;
            if (value.contains("name"))
                pass.Type = getPassTypeFromName(value["name"]);
            
            if (value.contains("strength"))
                pass.Strength = value["strength"];

            if (value.contains("intensity"))
                pass.Intensity = value["intensity"];
            
            if (value.contains("invert"))
                pass.RevCol = value["invert"];

            passes.push_back(pass);
        }

        Logger::PrintDebug(to_string(passes.size()) + string(" passes loaded."));
    }
    catch(exception& e)
    {
        Logger::PrintErr("Error occured while loading a file: '" + path + "': " + e.what());
        return passes;
    }

    return passes;
}

ConfigLoader::GlobalConfig ConfigLoader::GetGlobalConfig(string& path)
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

        stringstream buffer;
        buffer << f.rdbuf();
        string content = buffer.str();

        content = Utils::RemoveComments(content);

        json data = json::parse(content);

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

    globalConfig.LoadedCorrectly = true;
    return globalConfig;
}

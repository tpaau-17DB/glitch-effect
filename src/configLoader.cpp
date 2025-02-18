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

const unordered_map<string, PassType> STR_TO_PASS =
{
    {"horizontal_distort", HORIZONTAL_DISTORT},
    {"discard", DISCARD},
    {"character_shuffle", CHARACTER_SHUFFLE},
    {"rand_offset_x", HORIZONTAL_OFFSET}
};

vector<Pass> getPassesFromJSON(string& path)
{
    vector<Pass> passes = vector<Pass>();

    path = expandPath(path);

    PrintDebug("Loading JSON file '" + path + "'.");

    try
    {
        ifstream f(path);

        if (!f.is_open()) 
        {
            PrintErr("Could not open the file!");
            return passes;
        }

        stringstream buffer;
        buffer << f.rdbuf();
        string content = buffer.str();

        json data = json::parse(content, nullptr, true, true);

        for (auto& [key, value] : data.items()) 
        {
            if (key == "global_config") continue;

            Pass pass;

            if (value.contains("name"))
            {
                if (STR_TO_PASS.count(value["name"]))
                {
                    pass.type = STR_TO_PASS.at(value["name"]);
                }
                else
                {
                    PrintErr("Unknown pass " + to_string(value["name"]) + "!");
                    pass.type = UNDEFINED;
                }
            }

            if (value.contains("strength"))
                pass.strength = value["strength"];

            if (value.contains("intensity"))
                pass.intensity = value["intensity"];

            if (value.contains("ignore_spaces"))
                pass.ignoreSpaces = value["ignore_spaces"];

            passes.push_back(pass);
        }

        PrintDebug(to_string(passes.size()) + string(" passes loaded."));
    }
    catch(exception& e)
    {
        PrintErr("Error occurred while loading a file: '" + path + "': " + e.what());
        return passes;
    }

    return passes;
}

GlobalConfig getGlobalConfig(string& path)
{
    GlobalConfig globalConfig;
    string tmp;

    PrintDebug("Loading JSON file '" + path + "'.");

    try
    {
        ifstream f(path);
        if (!f.is_open()) 
        {
            PrintErr("Could not open the file!");
            return globalConfig;
        }

        stringstream buffer;
        buffer << f.rdbuf();
        string content = buffer.str();

        json data = json::parse(content, nullptr, true, true);

        if (data.contains("global_config"))
        {
            json globalData = data["global_config"];

            if (globalData.contains("sleeptime_ms"))
                globalConfig.sleeptimeMS = globalData["sleeptime_ms"];

            if (globalData.contains("logger_verbosity"))
                globalConfig.loggerVerbosity = globalData["logger_verbosity"];

            if (globalData.contains("default_ascii_path"))
                globalConfig.defaultAsciiPath = globalData["default_ascii_path"];

            if (globalData.contains("chromatic_aberration"))
                globalConfig.chromaticAberrationEnabled = globalData["chromatic_aberration"];

            if (globalData.contains("foreground_color"))
            {
                tmp = globalData["foreground_color"];
                if (isValidColorID(strToColorID(tmp)))
                    globalConfig.foregroundColorStr = tmp;
                else
                    PrintErr("Invalid color ID: " + tmp);
            }

            if (globalData.contains("background_color"))
            {
                tmp = globalData["background_color"];
                if (isValidColorID(strToColorID(tmp)))
                    globalConfig.backgroundColorStr = tmp;
                else
                    PrintErr("Invalid color ID: " + tmp);
            }
        }
        else
        {
            PrintLog("No 'global_config' object in JSON file '" + path + "'.");        
        }

        PrintDebug("Finished loading config.");
    }
    catch(const exception& e)
    {
        PrintErr("Error occurred while loading a file: '" + path + "': " + e.what());
        return globalConfig;
    }

    globalConfig.loadedCorrectly = true;
    return globalConfig;
}

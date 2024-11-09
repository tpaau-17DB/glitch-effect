#include <exception>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "Logger.h"
#include "PassLoader.h"

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

PassLoader::PassType getPassTypeFromName(const string& name)
{
    PassLoader::PassType type = PassLoader::Undefined;

    unsigned long hash = hashString(name);

    //Logger::PrintDebug(to_string(hash));

    switch(hash)
    {
        case 3059108212:  // 'vertical distort'
            type = PassLoader::VerticalDistort;
            break;

        default:
            Logger::PrintErr("Unknown pass type: '" + name + "'.");
            break;
    }

    return type;
}

vector<PassLoader::pass> PassLoader::GetPassesFromJSON(const string& path)
{
    vector<PassLoader::pass> passes = vector<PassLoader::pass>();

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
        Logger::PrintDebug("Parsed JSON data:");

        for (auto& [key, value] : data.items()) 
        {
            string name = value["name"];
            int strength = value["strength"];
            int intensity = value["intensity"];
            bool invert = value["invert"];
            
            PassLoader::pass pass;
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

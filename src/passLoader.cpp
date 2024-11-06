#include <exception>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "Logger.h"
#include "PassLoader.h"

using namespace std;

using json = nlohmann::json;

int PassLoader::GetPassesFromJSON(const string& path, vector<PassLoader::pass>&)
{
    vector<PassLoader::pass> passes = vector<PassLoader::pass>();

    Logger::PrintDebug("Loading a JSON file '" + path + "'.");

    try
    {
        std::ifstream f(path);
        json data = json::parse(f);
    }
    catch(exception& e)
    {
        Logger::PrintErr("Error occured while loading a file: '" + path + "': " + e.what());
        return 1;
    }

    return 0;
}

#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "ConfigLoader.h"
#include "Logger.h"

using namespace std;

int ConfigLoader::GetParamFromConf(map<string, int>* conf, const string &param)
{
    auto it = conf->find(param);

    if (it != conf->end())
    {
        Logger::PrintLog("Param '" + param + "' = " + to_string(it->second));
        return it->second;
    } 
    else
    {
        Logger::PrintWarn("Param '" + param + "' not found in config file.");
        return -1;
    }
}

map<string, int> ConfigLoader::LoadConf(const string& filename) {
    map<string, int> config;
    ifstream file(filename);
    config["exit-code"] = 0;

    if (!file.is_open()) 
    {
	Logger::PrintErr("Failed to open the config file at '" + filename + "'!", 0);
	config["exit-code"] = 1;
    }

    string line;
    while (getline(file, line)) 
    {
        istringstream iss(line);
        string key;

        if (getline(iss, key, '=')) 
	{
            string valueStr;

            if (getline(iss, valueStr)) 
	    {
                try 
		{
                    cout<<"config-loader.cpp line 37!"<<endl;
                    int value = stoi(valueStr);
                    config[key] = value;
                } 
		catch (const invalid_argument& e) 
		{
		    Logger::PrintErr("Invalid value for key " + key + ": " + valueStr, 0);
		    config["exit-code"] = 1;
                } 
		catch (const out_of_range& e) 
		{
		    Logger::PrintErr("Value out of range for key " + key + ": " + valueStr, 0);
		    config["exit-code"] = 1;
                }
            }
        }
    }

    file.close();
    return config;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "ConfigLoader.h"

using namespace std;

map<string, int> ConfigLoader::LoadConf(const string& filename) {
    map<string, int> config;
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Could not open config file");
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
                    int value = stoi(valueStr);
                    config[key] = value;
                } 
				catch (const invalid_argument& e) 
				{
                    cerr << "Invalid value for key " << key << ": " << valueStr << endl;
                } 
				catch (const out_of_range& e) 
				{
                    cerr << "Value out of range for key " << key << ": " << valueStr << endl;
                }
            }
        }
    }

    file.close();
    return config;
}

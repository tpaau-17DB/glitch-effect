#include "ConfigLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

ConfigLoader::ConfigLoader(const string& filename) {
    loadConfig(filename);
}

void ConfigLoader::loadConfig(const string& filename) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Unable to open config file: " + filename);
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        if (getline(iss, key, '=')) {
            string value;
            if (getline(iss, value)) {
                configMap[key] = value;
            }
        }
    }
}

string ConfigLoader::get(const string& key) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    } else {
        throw runtime_error("Key not found in config: " + key);
    }
}

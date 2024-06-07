#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <string>
#include <map>

class ConfigLoader {
public:
    ConfigLoader(const string& filename);
    string get(const string& key) const;

private:
    void loadConfig(const string& filename);
    map<string, string> configMap;
};

#endif // CONFIGLOADER_H

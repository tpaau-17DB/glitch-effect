#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

class ConfigLoader
{
    public:
	static std::map<std::string, int> LoadConf(const std::string &path);
        static int GetParamFromConf(std::map<std::string, int>* conf, const std::string& param);
};

#endif

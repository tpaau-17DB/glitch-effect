#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class FileLoader
{
    public:
        static std::string GetXDG_CONFIG_HOME();
        static std::vector<std::string> GetLines(std::string &path);
        static bool CheckIfFileExists(const std::string &path);
        static std::string LookForConfigFiles();
        static std::string ExpandPath(const std::string &path);
};

#endif

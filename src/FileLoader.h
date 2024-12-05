#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class FileLoader
{
    public:
	static std::vector<std::string> GetLines(std::string &path);
        static bool CheckIfFileExists(const std::string &path);
        static std::string LookForConfigFiles();
        static std::string ExpandPath(const std::string &path);

    private:
        /*
         * Paths where the config file could be stored
         * The first entry has the most importance and will be checked first
         * The lats entry has the least importance and will be checked last
        */
        static const std::string configPaths[];
        
};

#endif

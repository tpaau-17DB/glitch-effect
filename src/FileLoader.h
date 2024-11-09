#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class FileLoader
{
    public:
	static std::vector<std::string> GetLines(const std::string &path);
        static bool CheckIfFileExists(const std::string &path);
        static std::string LookForConfigFiles();

    private:
        /*
         * Paths where the config file could be stored
         * The first entry has the most importance and will be checked first
         * The lats entry has the least importance and will be checked last
        */
        static const std::string configPaths[];
        
};

#endif

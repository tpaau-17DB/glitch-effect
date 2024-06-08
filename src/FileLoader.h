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
};

#endif

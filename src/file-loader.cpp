#include "FileLoader.h"

#include <iostream>
#include <vector>
#include <fstream>

std::vector<std::string> FileLoader::GetLines(const std::string &path)
{
    std::vector<std::string> lines;
	std::ifstream file(path);

    if (!file.good())
    {
        throw std::runtime_error("File error! File path: '" + path + "'");
    }

    if (file.is_open())
    {
        std::string line;

        while (getline(file, line))
        {
            lines.push_back(line);
        }

        file.close();
    }
    else
    {
		std::cerr << "Unable to open file: " << path << std::endl;
    }

    return lines;
}

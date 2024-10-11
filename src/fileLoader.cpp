#include <vector>
#include <fstream>

#include "FileLoader.h"
#include "Logger.h"

using namespace std;

vector<string> FileLoader::GetLines(const string &path)
{
    vector<string> lines;
    ifstream file(path);

    if (!file.good())
    {
        Logger::PrintErr("File error! File path: '" + path + "'");
    }

    if (file.is_open())
    {
        string line;

        while (getline(file, line))
        {
            lines.push_back(line);
        }

        file.close();
    }
    else
    {
	Logger::PrintErr("Unable to open file: '" + path + "'");
    }

    return lines;
}

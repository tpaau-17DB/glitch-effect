#include <stdexcept>
#include <regex>
#include <string>
#include <unordered_map>
#include <random>
#include <vector>
#include <regex>

#include "Logger.h"
#include "Utils.h"
#include "Printer.h"

using namespace std;

random_device rd;

bool isPrecalculated;
unsigned int currentPrecalculated;
vector<int> precalculatedInts;

const regex ANSICodesRegex("\033\\[[0-9;]*[mKHFfLs]?");

const unordered_map<string, Color> STR_TO_COLOR =
{
    {"none", NONE_COLOR},
    {"black", BLACK},
    {"blue", BLUE},
    {"cyan", CYAN},
    {"green", GREEN},
    {"magenta", MAGENTA},
    {"red", RED},
    {"white", WHITE},
    {"yellow", YELLOW}
};

void precalculateInts()
{
    for (int i = 0; i < 100; i++)
    {
        precalculatedInts.push_back(getRandomInt(0, 100));
    }
    isPrecalculated = true;
    PrintDebug("Random shorts precalculated.");
}

int strToInt(const string& str)
{
    int result;

    try
    {
        result = stoi(str);
    }
    catch (const invalid_argument& e) 
    {
        result = -1;
        PrintErr(string("Failed to convert '") + str + string("' to an int. (") + e.what() + string(")"));
    }
    return result;
}

int strToColorID(const string& str)
{
    if (STR_TO_COLOR.count(toLower(str)))
    {
        return STR_TO_COLOR.at(toLower(str));
    }
    else
    {
        PrintErr("Invalid color: '" + str + "'.");
        return NONE_COLOR;
    }
}

float getRandomFloat(const float min, const float max)
{
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int getRandomInt(const int min, const int max)
{
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

int getRandomPrecalculatedInt()
{
    if (!isPrecalculated)
        precalculateInts();
    
    currentPrecalculated++;
    if (currentPrecalculated >= precalculatedInts.size())
        currentPrecalculated = 0;

    return precalculatedInts[currentPrecalculated];
}

// No support for ANSI codes yet :P
vector<string> removeANSICodes(const vector<string>& lines)
{
    vector<string> filteredLines = vector<string>();

    for (unsigned int i = 0; i < lines.size(); i++)
    {
        string filtered = regex_replace(lines[i], ANSICodesRegex, "");
        filteredLines.push_back(filtered);
    }
    return filteredLines;
}

string toLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

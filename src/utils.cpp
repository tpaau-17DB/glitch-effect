#include <stdexcept>
#include <regex>
#include <string>
#include <random>
#include <vector>
#include <regex>

#include "ConfigLoader.h"
#include "Logger.h"
#include "Utils.h"
#include "Printer.h"

using namespace std;

random_device rd;

bool isPrecalculated;
unsigned int currentPrecalculated;
vector<unsigned short> precalculatedShorts;

regex ANSICodesRegex("\033\\[[0-9;]*[mKHFfLs]?");

void precalculateShorts()
{
    for (int i = 0; i < 100; i++)
    {
        precalculatedShorts.push_back(Utils::GetRandomShort(0, 100));
    }
    isPrecalculated = true;
    Logger::PrintDebug("Random shorts precalculated.");
}

int Utils::StrToInt(const string& str)
{
    int result;

    try
    {
        result = stoi(str);
    }
    catch (const invalid_argument& e) 
    {
        result = -1;
        Logger::PrintErr(string("Failed to convert '") + str + string("' to an int. (") + e.what() + string(")"));
    }
    return result;
}

int Utils::StrToColorID(const string& str)
{
    unsigned long hash = HashString(str);

    Logger::PrintDebug("\n");
    
    Logger::PrintDebug("none: " + to_string(HashString("none")));
    Logger::PrintDebug("black: " + to_string(HashString("black")));
    Logger::PrintDebug("red: " + to_string(HashString("red")));
    Logger::PrintDebug("green: " + to_string(HashString("green")));
    Logger::PrintDebug("yellow: " + to_string(HashString("yellow")));
    Logger::PrintDebug("blue: " + to_string(HashString("blue")));
    Logger::PrintDebug("magenta: " + to_string(HashString("magenta")));
    Logger::PrintDebug("cyan: " + to_string(HashString("cyan")));
    Logger::PrintDebug("white: " + to_string(HashString("white")));

    Logger::PrintDebug("\n");


    switch(hash)
    {
        case 2087865487:
            return Printer::NONE;

        case 175804258:
            return Printer::BLACK;

        case 193432438:
            return Printer::RED;

        case 172398782:
            return Printer::GREEN;

        case 2241670145:
            return Printer::YELLOW;

        case 2087736443:
            return Printer::BLUE;

        case 1065710736:
            return Printer::MAGENTA;

        case 2087790096:
            return Printer::CYAN;

        case 191284034:
            return Printer::WHITE;

        default:
            return -2;
    }
}

unsigned long Utils::HashString(const string& str)
{
    unsigned int hash = 5381;
    for (char c : str)
    {
        hash = (hash * 33) ^ c;
    }
    return hash;
}

float Utils::GetRandomFloat(const float min, const float max)
{
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

unsigned short Utils::GetRandomShort(const unsigned short min, const unsigned short max)
{
    mt19937 gen(rd());
    uniform_int_distribution<unsigned short> dist(min, max);
    return dist(gen);
}

unsigned short Utils::GetRandomPrecalculatedShort()
{
    if (!isPrecalculated)
        precalculateShorts();
    
    currentPrecalculated++;
    if (currentPrecalculated >= precalculatedShorts.size())
        currentPrecalculated = 0;

    return precalculatedShorts[currentPrecalculated];
}

ConfigLoader::PassType Utils::GetPassTypeFromName(const string& name)
{
    ConfigLoader::PassType type = ConfigLoader::Undefined;

    unsigned long hash = Utils::HashString(name);

    type = ConfigLoader::Undefined;

    switch(hash)
    {
        case 3091456344:  // 'horizontal distort'
            type = ConfigLoader::HorizontalDistort;
            break;

        case 2811326735:  // 'discard'
            type = ConfigLoader::Discard;
            break;

        case 409254235:  // 'character shuffle'
            type = ConfigLoader::CharacterShuffle;
            break;

        default:
            Logger::PrintErr("Unknown pass type: '" + name + "'.");
            break;
    }

    return type;
}

vector<string> Utils::RemoveANSICodes(const vector<string>& lines)
{
    vector<string> filteredLines = vector<string>();

    for (unsigned int i = 0; i < lines.size(); i++)
    {
        string filtered = regex_replace(lines[i], ANSICodesRegex, "");
        filteredLines.push_back(filtered);
    }
    return filteredLines;
}

string Utils::RemoveComments(const string& str)
{
    string result = str;

    result = regex_replace(result, std::regex("//[^\n]*"), "");
    result = regex_replace(result, std::regex("/\\*[\\s\\S]*?\\*/"), "");

    return result;
}

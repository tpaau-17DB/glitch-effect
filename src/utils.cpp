#include "Logger.h"
#include "Utils.h"
#include "Printer.h"

using namespace std;

unsigned long Utils::HashString(const string& str)
{
    unsigned int hash = 5381;
    for (char c : str)
    {
        hash = (hash * 33) ^ c;
    }
    return hash;
}

int Utils::StrToColorID(const string& str)
{
    unsigned long hash = HashString(str);

    switch(hash)
    {
        case 2088914255:
            return Printer::NONE;

        case 210193794:
            return Printer::BLACK;

        case 193463958:
            return Printer::RED;

        case 209230046:
            return Printer::GREEN;

        case 3455011137:
            return Printer::YELLOW;

        case 2088778555:
            return Printer::BLUE;

        case 2448879856:
            return Printer::MAGENTA;

        case 2088904208:
            return Printer::CYAN;

        case 22811510:
            return Printer::WHITE;

        default:
            return -2;
    }
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

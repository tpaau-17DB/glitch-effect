#include <string>
#include <vector>
#include <ctime>
#include <string>

#include "Logger.h"
#include "ConfigLoader.h"
#include "AsciiBuffer.h"
#include "Utils.h"

using namespace std;

const string ESCAPE = "\033[0m";

vector<string> lines;
vector<string> distortedLines;

int maxLineLength;
int maxDistortedLineLength;


// Internals
int getMaxLineLengthFromVector(vector<string>& lines)
{
    int maxLength = 0;

    for (const string &str : lines)
    {
        if (static_cast<int>(str.length()) > maxLength)
            maxLength = str.length();
    }

    return maxLength;
}

void ensureDistortedLinesNotEmpty()
{
    if (distortedLines.size() == 0)
    {
        distortedLines = lines;
    }
}


// On create
AsciiBuffer::AsciiBuffer()
{
    lines = vector<string>();
    distortedLines = vector<string>();
    Logger::PrintDebug("Created AsciiBuffer instance.");
}

AsciiBuffer::AsciiBuffer(const vector<string> newLines)
{
    lines = Utils::RemoveANSICodes(newLines);
    distortedLines = vector<string>();
    Logger::PrintDebug("Created AsciiBuffer instance.");

    int maxLength = getMaxLineLengthFromVector(lines);

    maxLineLength = maxLength;
    maxDistortedLineLength = maxLength;
}


// On destroy
AsciiBuffer::~AsciiBuffer()
{
    Logger::PrintDebug("AsciiBuffer instance was destroyed.");
}


// Setters
void AsciiBuffer::OverwriteLines(const vector<string> newLines)
{
    lines = Utils::RemoveANSICodes(newLines);

    int maxLength = getMaxLineLengthFromVector(lines);

    maxLineLength = maxLength;
    maxDistortedLineLength = maxLength;
}

void AsciiBuffer::ClearAll()
{
    lines = vector<string>();
    distortedLines = vector<string>();

    maxLineLength = 0;
    maxDistortedLineLength = 0;
}

void AsciiBuffer::ResetDistorted()
{
    distortedLines = lines;
    maxDistortedLineLength = maxLineLength;
}


//Getters
int AsciiBuffer::GetMaxLineLength()
{
    return maxLineLength;
}

int AsciiBuffer::GetMaxDistortedLineLength()
{
    return maxDistortedLineLength;
}

int AsciiBuffer::GetLineCount()
{
    return lines.size();
}

vector<string> AsciiBuffer::GetLines()
{
    return lines;
}

vector<string> AsciiBuffer::GetDistortedLines()
{
    return distortedLines;
}

vector<string>* AsciiBuffer::GetDistortedLinesPtr()
{
    return &distortedLines;
}


// Filters
void AsciiBuffer::HorizontalDistort(const int intensity, const int strength)
{
    ensureDistortedLinesNotEmpty();
    
    int num;
    string spaces;

    for (string &str : distortedLines)
    {
        num = strength;

        int reverseEffect = rand() % 2;

        if (Utils::GetRandomFloat(0, 100) < intensity)
        {
            if (!reverseEffect)
            {
                num += rand() % strength + 1;
            }
            else
            {
                num -= rand() % strength + 1;
            }
        }

        spaces = string(num, ' ');
        str = spaces + str + spaces;
    }

    maxDistortedLineLength += (2 * strength);
}

void AsciiBuffer::ShuffleCharacters(const int intensity)
{
    ensureDistortedLinesNotEmpty();

    int dst;
    int length;
    for (string &str : distortedLines)
    {
        length = str.length();
        for (int src = 0; src < length; src++)
        {
            if (str[src] == ' ' || Utils::GetRandomPrecalculatedShort() >= intensity) continue;
            dst = (Utils::GetRandomPrecalculatedShort() % length);

            if (str[dst] == ' ') continue;

            char srcC = str[src];
            char dstC = str[dst];

            str[src] = dstC;
            str[dst] = srcC;
        }
    }
}


// Others
int AsciiBuffer::ApplyPasses(std::vector<ConfigLoader::pass> passes)
{
    if (passes.size() == 0)
    {
        Logger::PrintErr("Passes list was empty!");
        return 1;
    }

    for (ConfigLoader::pass& pass : passes)
    {
        switch(pass.Type)
        {
            case ConfigLoader::HorizontalDistort:
                this->HorizontalDistort(pass.Intensity, pass.Strength);
                break;

            case ConfigLoader::Discard:
                if (Utils::GetRandomFloat(0, 100) < pass.Intensity)
                {
                    distortedLines = vector<string>();
                    return 0;
                }
                break;

            case ConfigLoader::Undefined:
                Logger::PrintErr("Undefined pass type!");
                return 1;

            case ConfigLoader::CharacterShuffle:
                ShuffleCharacters(pass.Intensity);
                break;

            default:
                Logger::PrintErr("Unknown pass type!");
                return 1;
        }
    }
    return 0;
}

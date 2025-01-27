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

int maxDistortedLineSize;

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
    PrintDebug("Created AsciiBuffer instance.");
}

AsciiBuffer::AsciiBuffer(const vector<string> newLines)
{
    lines = removeANSICodes(newLines);
    distortedLines = vector<string>();
    PrintDebug("Created AsciiBuffer instance.");

    int maxLength = getMaxLineLengthFromVector(lines);

    maxLineLength = maxLength;
    maxDistortedLineLength = maxLength;
}


// On destroy
AsciiBuffer::~AsciiBuffer()
{
    PrintDebug("AsciiBuffer instance was destroyed.");
}


// Setters
void AsciiBuffer::overwriteLines(const vector<string> newLines)
{
    lines = removeANSICodes(newLines);

    int maxLength = getMaxLineLengthFromVector(lines);

    maxLineLength = maxLength;
    maxDistortedLineLength = maxLength;
}

void AsciiBuffer::clearAll()
{
    lines = vector<string>();
    distortedLines = vector<string>();

    maxLineLength = 0;
    maxDistortedLineLength = 0;
}

void AsciiBuffer::resetDistorted()
{
    distortedLines = lines;
    maxDistortedLineLength = maxLineLength;
    maxDistortedLineSize = lines.size();
}


//Getters
int AsciiBuffer::getMaxLineLength()
{
    return maxLineLength;
}

int AsciiBuffer::getMaxDistortedLineLength()
{
    return maxDistortedLineLength;
}

int AsciiBuffer::getMaxDistortedLineSize()
{
    return maxDistortedLineSize;
}

int AsciiBuffer::getLineCount()
{
    return lines.size();
}

vector<string> AsciiBuffer::getLines()
{
    return lines;
}

vector<string> AsciiBuffer::getDistortedLines()
{
    return distortedLines;
}

vector<string>* AsciiBuffer::getDistortedLinesPtr()
{
    return &distortedLines;
}


// Filters

// Add a random horizontal offset to the entire image
void AsciiBuffer::randOffsetX(const Pass data)
{
    if (getRandomInt(0, 100) >= data.intensity)
        return;

    string spaces = string(getRandomInt(0, data.strength * 2), ' ');

    for (string& line : distortedLines)
    {
        line = spaces + line + spaces;
    }

    maxDistortedLineLength += (2 * data.strength);
}

// Add a random horizontal offset to individual lines
void AsciiBuffer::horizontalDistort(const Pass data)
{
    string spaces;
    int distortion;
    for (string &line : distortedLines)
    {
        if (getRandomInt(0, 100) < data.intensity)
            distortion = getRandomInt(0, data.strength * 2);
        else
            distortion = data.strength;

        spaces = string(distortion, ' ');
        line = spaces + line + spaces;
    }

    maxDistortedLineLength += (2 * data.strength);
}

// Shuffle images' characters randomly
// This is the performance version, utilizing precomputed pseudorandom numbers
void AsciiBuffer::shuffleCharacters(const Pass data)
{
    int dst;
    int length;
    for (string &line : distortedLines)
    {
        length = line.length();
        for (int src = 0; src < length; src++)
        {
            if (getRandomPrecalculatedInt() >= data.intensity
                    || (data.ignoreSpaces && line[src] == ' '))
            {
                continue;
            }
            dst = (getRandomPrecalculatedInt() % length);

            if (line[dst] == ' ') continue;

            char srcC = line[src];
            char dstC = line[dst];

            line[src] = dstC;
            line[dst] = srcC;
        }
    }
}


// Apply the passes to the buffer
int AsciiBuffer::applyPasses(vector<Pass> passes)
{
    if (passes.size() == 0)
    {
        PrintErr("Passes list was empty!");
        return 1;
    }

    ensureDistortedLinesNotEmpty();
    for (Pass& pass : passes)
    {
        switch(pass.type)
        {
            case HORIZONTAL_DISTORT:
                horizontalDistort(pass);
                break;

            case DISCARD:
                // This would not be efficient as a separate function
                if (getRandomFloat(0, 100) < pass.intensity)
                {
                    distortedLines = vector<string>();
                    return 0;
                }
                break;

            case CHARACTER_SHUFFLE:
                shuffleCharacters(pass);
                break;

            case HORIZONTAL_OFFSET:
                AsciiBuffer::randOffsetX(pass);
                break;

            case UNDEFINED:
                PrintErr("Undefined pass type!");
                return 1;

            default:
                PrintErr("Unknown pass type!");
                return 1;
        }
    }
    return 0;
}

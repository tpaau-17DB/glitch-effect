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
    maxDistortedLineSize = lines.size();
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

int AsciiBuffer::GetMaxDistortedLineSize()
{
    return maxDistortedLineSize;
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

// Add a random horizontal offset to the entire image
void AsciiBuffer::RandOffsetX(const int intensity, const int strength)
{
    if (Utils::GetRandomInt(0, 100) >= intensity)
        return;

    string spaces = string(Utils::GetRandomInt(0, strength * 2), ' ');

    for (string& line : distortedLines)
    {
        line = spaces + line + spaces;
    }

    maxDistortedLineLength += (2 * strength);
}

// Add a random horizontal offset to individual lines
void AsciiBuffer::HorizontalDistort(const int intensity, const int strength)
{
    string spaces;
    int distortion;

    for (string &line : distortedLines)
    {
        if (Utils::GetRandomInt(0, 100) < intensity)
            distortion = Utils::GetRandomInt(0, strength * 2);
        else
            distortion = strength;

        spaces = string(distortion, ' ');
        line = spaces + line + spaces;
    }

    maxDistortedLineLength += (2 * strength);
}

// Shuffle images' characters randomly
// This is the performance version, utilizing precomputed pseudorandom numbers
void AsciiBuffer::ShuffleCharacters(const int intensity)
{
    int dst;
    int length;
    for (string &line : distortedLines)
    {
        length = line.length();
        for (int src = 0; src < length; src++)
        {
            if (line[src] == ' ' || Utils::GetRandomPrecalculatedShort() >= intensity)
                continue;
            dst = (Utils::GetRandomPrecalculatedShort() % length);

            if (line[dst] == ' ') continue;

            char srcC = line[src];
            char dstC = line[dst];

            line[src] = dstC;
            line[dst] = srcC;
        }
    }
}


// Apply the passes to the buffer
int AsciiBuffer::ApplyPasses(std::vector<ConfigLoader::pass> passes)
{
    if (passes.size() == 0)
    {
        Logger::PrintErr("Passes list was empty!");
        return 1;
    }

    ensureDistortedLinesNotEmpty();
    for (ConfigLoader::pass& pass : passes)
    {
        switch(pass.Type)
        {
            case ConfigLoader::HorizontalDistort:
                this->HorizontalDistort(pass.Intensity, pass.Strength);
                break;

            case ConfigLoader::Discard:
                // This would not be efficient to implement as a separate function
                if (Utils::GetRandomFloat(0, 100) < pass.Intensity)
                {
                    distortedLines = vector<string>();
                    return 0;
                }
                break;

            case ConfigLoader::CharacterShuffle:
                ShuffleCharacters(pass.Intensity);
                break;

            case ConfigLoader::HorizontalOffset:
                AsciiBuffer::RandOffsetX(pass.Intensity, pass.Strength);
                break;

            case ConfigLoader::Undefined:
                Logger::PrintErr("Undefined pass type!");
                return 1;

            default:
                Logger::PrintErr("Unknown pass type!");
                return 1;
        }
    }
    return 0;
}

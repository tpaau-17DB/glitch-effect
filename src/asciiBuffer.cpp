#include <string>
#include <vector>
#include <ctime>
#include <string>

#include "Logger.h"
#include "PassLoader.h"
#include "AsciiBuffer.h"

using namespace std;

const string AsciiBuffer::ESCAPE = "\033[0m";

// On create
AsciiBuffer::AsciiBuffer()
{
    lines = vector<string>();
    distortedLines = vector<string>();
    Logger::PrintDebug("AsciiBuffer: Instance created.");
}

AsciiBuffer::AsciiBuffer(const vector<string> lines)
{
    this->lines = lines;
    distortedLines = vector<string>();
    Logger::PrintDebug("AsciiBuffer: Instance created.");

    int max_length = 0;

    for (const string &str : lines)
    {
        if (static_cast<int>(str.length()) > max_length)
            max_length = str.length();
    }

    maxLineLength = max_length;
    maxDistortedLineLength = max_length;
}


// On destroy
AsciiBuffer::~AsciiBuffer()
{
    Logger::PrintDebug("AsciiBuffer: Instance destroyed.");
}


// Setters

void AsciiBuffer::AddLine(const string line)
{
    this->lines.push_back(line);
}

void AsciiBuffer::AddLines(const vector<string> lines)
{
    vector<string> result = vector<string>();

    result.reserve(this->lines.size() + lines.size());
    result.insert(result.end(), this->lines.begin(), this->lines.end());
    result.insert(result.end(), lines.begin(), lines.end());

    this->lines = result;
}

void AsciiBuffer::OverwriteLines(const vector<string> lines)
{
    this->lines = lines;

    int max_length = 0;

    for (const string &str : lines)
    {
        if (static_cast<int>(str.length()) > max_length)
            max_length = str.length();
    }

    maxLineLength = max_length;
    maxDistortedLineLength = max_length;
}

void AsciiBuffer::ClearAll()
{
    this->lines = vector<string>();
    this->distortedLines = vector<string>();
}

void AsciiBuffer::ResetDistorted()
{
    distortedLines = lines;
    maxDistortedLineLength = maxLineLength;
}


//Getters

int AsciiBuffer::GetMaxDistortedLength()
{
    return maxDistortedLineLength;
}

int AsciiBuffer::GetLineCount()
{
    return this->lines.size();
}

int AsciiBuffer::GetMaxLength()
{
    return maxLineLength;
}

vector<string> AsciiBuffer::GetLines()
{
    return this->lines;
}

vector<string>* AsciiBuffer::GetDistortedLinesPtr()
{
    return &this->distortedLines;
}

vector<string> AsciiBuffer::GetDistortedLines()
{
    return distortedLines;
}


// Filters
void AsciiBuffer::VerticalDistort(const int intensity, const int strength)
{
    ensureDistortedLinesNotEmpty();
    
    int num;
    string spaces;

    for (string &str : distortedLines)
    {
        num = strength;

        int rev_effect = rand() % 2;

        if ((rand() % intensity + 1) == 1)
        {
            if (!rev_effect)
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


// Others
void AsciiBuffer::ApplyPasses(std::vector<PassLoader::pass> passes)
{
    if (passes.size() == 0)
    {
        Logger::PrintErr("Passes list was empty!");
        return;
    }

    for (PassLoader::pass& pass : passes)
    {
        switch(pass.PT)
        {
            case PassLoader::VerticalDistort:
                this->VerticalDistort(pass.PP.Intensity, pass.PP.Strength);
                break;

            case PassLoader::Undefined:
                Logger::PrintErr("Undefined pass type!");
                break;

            default:
                Logger::PrintErr("Undefined pass type!");
                break;
        }
    }
}


// Internals
void AsciiBuffer::ensureDistortedLinesNotEmpty()
{
    if (distortedLines.size() == 0)
    {
        Logger::PrintDebug("AsciiBuffer: Distorted lines buffer is empty. Copying lines to distortedLines.");
        distortedLines = lines;
    }
}

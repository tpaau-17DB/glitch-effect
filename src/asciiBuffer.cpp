#include <string>
#include <vector>
#include <ctime>
#include <string>

#include "Logger.h"
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
}

void AsciiBuffer::ClearAll()
{
    this->lines = vector<string>();
}

void AsciiBuffer::ResetDistorted()
{
    distortedLines = lines;
}


//Getters

int AsciiBuffer::GetLineCount()
{
    return this->lines.size();
}

int AsciiBuffer::GetMaxLength()
{
    
    int max_length = 0;

    for (const string &str : lines)
    {
        if (static_cast<int>(str.length()) > max_length)
            max_length = str.length();
    }

    return max_length;
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
    
    string spaces;
    int num;
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
}

// Do not delete
/*void AsciiBuffer::ChromaticAberration(const int intensity, const string colorLeftANSI, const string colorRightANSI)*/
/*{*/
/*    ensureDistortedLinesNotEmpty();*/
/**/
/*    for (string &str : distortedLines)*/
/*    {*/
/*        string modified = "";*/
/**/
/*        for (size_t i = 0; i < str.length(); ++i)*/
/*        {*/
/*            if (str[i] == ' ') */
/*            {*/
/*                if (i > 0 && i < str.length() - 1 && (str[i - 1] != ' ' || str[i + 1] != ' ')) */
/*                {*/
/*                    if (str[i + 1] != ' ')*/
/*                    {*/
/*                        modified += colorLeftANSI + str[i + 1] + ESCAPE;*/
/*                    }*/
/*                    else*/
/*                    {*/
/*                        modified += colorRightANSI + str[i - 1] + ESCAPE;*/
/*                    }*/
/*                }*/
/*                else*/
/*                {*/
/*                    modified += str[i];*/
/*                }*/
/*            }*/
/*            else*/
/*            {*/
/*                modified += str[i];*/
/*            }*/
/*        }*/
/**/
/*        modified += ".";*/
/*        str = modified;*/
/**/
/*    }*/
/*}*/


// Internals

void AsciiBuffer::ensureDistortedLinesNotEmpty()
{
    if (distortedLines.size() == 0)
    {
        Logger::PrintDebug("AsciiBuffer: Distorted lines buffer is empty. Copying lines to distortedLines.");
        distortedLines = lines;
    }
}

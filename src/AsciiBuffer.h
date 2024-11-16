#ifndef ASCII_BUFFER_H
#define ASCII_BUFFER_H

#include <vector>
#include <string>

#include "ConfigLoader.h"

class AsciiBuffer
{
    public:
        AsciiBuffer();
        AsciiBuffer(const std::vector<std::string> lines);
        ~AsciiBuffer();

        // Setters
        void AddLine(const std::string line);
        void AddLines(const std::vector<std::string> lines);
        void OverwriteLines(const std::vector<std::string> lines);
        void ClearAll();
        void ResetDistorted();

        // Getters
        int GetLineCount();
        int GetMaxLineLength();
        int GetMaxDistortedLineLength();
        std::vector<std::string> GetLines();
        std::vector<std::string> GetDistortedLines();
        std::vector<std::string>* GetDistortedLinesPtr();

        // Filters
        void VerticalDistort(const int intensity, const int strength);

        // Other
        int ApplyPasses(std::vector<ConfigLoader::pass> passes);

    private:
        int maxDistortedLineLength;
        int maxLineLength;

        std::vector<std::string> lines;
        std::vector<std::string> distortedLines;
        static const std::string ESCAPE;

        void ensureDistortedLinesNotEmpty();
        int getMaxLineLengthFromVector(std::vector<std::string>& lines);
};

#endif

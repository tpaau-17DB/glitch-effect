#ifndef ASCII_BUFFER_H
#define ASCII_BUFFER_H

#include <vector>
#include <string>

#include "PassLoader.h"

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
        int GetMaxLength();
        int GetMaxDistortedLength();
        std::vector<std::string> GetLines();
        std::vector<std::string> GetDistortedLines();
        std::vector<std::string>* GetDistortedLinesPtr();

        // Filters
        void VerticalDistort(const int intensity, const int strength);

        // Other
        void ApplyPasses(std::vector<PassLoader::pass> passes);

    private:
        int maxDistortedLineLength;
        int maxLineLength;

        std::vector<std::string> lines;
        std::vector<std::string> distortedLines;
        static const std::string ESCAPE;

        void ensureDistortedLinesNotEmpty();
};

#endif

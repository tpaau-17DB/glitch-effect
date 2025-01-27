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
        void addLine(const std::string line);
        void overwriteLines(const std::vector<std::string> lines);
        void clearAll();
        void resetDistorted();

        // Getters
        int getLineCount();
        int getMaxLineLength();
        int getMaxDistortedLineLength();
        int getMaxDistortedLineSize();
        std::vector<std::string> getLines();
        std::vector<std::string> getDistortedLines();
        std::vector<std::string>* getDistortedLinesPtr();

        // Filters
        void randOffsetX(const Pass data);
        void horizontalDistort(const Pass data);
        void shuffleCharacters(const Pass data);

        // Other
        int applyPasses(std::vector<Pass> passes);
};

#endif

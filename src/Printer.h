#ifndef PRINTER_H
#define PRINTER_H

#include<string>
#include<vector>

#include "AsciiBuffer.h"

class Printer
{
    public:
        enum Color
        {
            NONE = -1,
            BLACK = 0,
            RED = 1,
            GREEN = 2,
            YELLOW = 3,
            BLUE = 4,
            MAGENTA = 5,
            CYAN = 6,
            WHITE = 7
        };

        // START & STOP
        static void Init(const int sleeptimeMS, const int offsetX, const int offsetY);
        static void Stop();

        // SETTERS
        static void SetDefaultColors(const Color fg, const Color bg);
        static void SetDefaultForegroundColor(const Color color);
        static void SetDefaultBackgroundColor(const Color color);

        // PRINTING
        static void Print(AsciiBuffer &buffer, const bool chromaticAberration);

        // UTILITY
        static bool IsValidColorID(const int colorID);

        static const std::vector<int> ColorRange;
};

#endif

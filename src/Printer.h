#ifndef PRINTER_H
#define PRINTER_H

#include<string>
#include<vector>

#include "AsciiBuffer.h"

enum Color
{
    NONE_COLOR = -1,
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
void initPrinter(const int sleeptimeMS);
void stopPrinter();

// SETTERS
void setForegroundColor(const Color color);
void setBackgroundColor(const Color color);

// PRINTING
void printDistortedText(AsciiBuffer &buffer, const bool chromaticAberration);

// UTILITY
bool isValidColorID(const int colorID);

#endif

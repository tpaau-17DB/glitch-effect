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

        static void init(const int sleeptimeMS, const int offsetX, const int offsetY);

        static void SetColors(const Color fg, const Color bg);

        static void print(AsciiBuffer &buffer, const int maxVerticalDistortion);

    private:
        static int sleeptimeMS;
        static int offsetX;
        static int offsetY;

        static bool autocenter;
};

#endif

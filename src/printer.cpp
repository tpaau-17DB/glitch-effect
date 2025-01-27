#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include <string>

#include "Printer.h"
#include "AsciiBuffer.h"
#include "Logger.h"

using namespace std;

const string windowTooSmallMessage = "It's a little bit claustrophobic in here...";

int sleeptimeMS = 40;

Color fgColor = NONE_COLOR;
Color bgColor = NONE_COLOR;

const vector<int> ColorRange = {-1, 7}; 

ostringstream customLogStream;

void initPrinter(const int setSleeptimeMS)
{
    sleeptimeMS = setSleeptimeMS;

    SetLogOutputStream(&customLogStream);
    ToggleUseCustomOutputStream(true);

    setlocale(LC_ALL, "C.UTF-8");
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    start_color();
    use_default_colors();
    curs_set(0);
}

void stopPrinter()
{
    PrintDebug("Stopping ncurses.");
    clear();
    curs_set(1);
    reset_prog_mode();
    endwin();

    FlushLogStream();
    ToggleUseCustomOutputStream(false);
}

// SETTERS
void setForegroundColor(const Color color)
{
    fgColor = color;
}

void setBackgroundColor(const Color color)
{
    bgColor = color;
}


// UTILITY
bool isValidColorID(const int colorID)
{
    if (colorID >= ColorRange[0] && colorID <= ColorRange[1])
        return true;
    return false;
}

// PRINTING FUNCTIONS
void printDistortedText(AsciiBuffer &buffer, const bool chromaticAberration)
{
    init_pair(1, fgColor, bgColor);
    int maxX, maxY;
    int offsetX = 0;
    int offsetY = 0;
    vector<string>* lines = buffer.getDistortedLinesPtr();

    getmaxyx(stdscr, maxY, maxX);

    if (buffer.getMaxDistortedLineLength() > maxX - offsetX
            || buffer.getMaxDistortedLineSize() > maxY - offsetY)
    {
        clear();
        move(maxY / 2, maxX / 2 - windowTooSmallMessage.length() / 2);
        printw("%s" , windowTooSmallMessage.c_str());
        refresh();
        usleep(100000);
        return;
    }

    clear();
    attron(COLOR_PAIR(1));
    bkgd(COLOR_PAIR(1));

    int x = offsetX;
    int y = offsetY;

    x = 0.5 * (maxX - buffer.getMaxDistortedLineLength());
    y = 0.5 * (maxY - lines->size());

    if (!chromaticAberration)
    {
        int i = 0;
        for (const string &line : *lines)
        {
            move(y + i, x);
            printw("%s", line.c_str());
            i++;
        }
    }
    else
    {
        init_pair(2, COLOR_RED, bgColor);
        init_pair(3, COLOR_BLUE, bgColor);

        string buf;
        int size;
        int pair;
        bool passed;
        int i = 0;
        for (const string &line : *lines)
        {
            size = line.size() - 1;
            for (int j = 0; j < size; j++)
            {
                passed = false;
                if (line[j] != ' ')
                {
                    if (j > 0 && line[j - 1] == ' ')
                    {
                        passed = true;
                        pair = 2;
                    }
                    else if (j < size && line[j + 1] == ' ')
                    {
                        passed = true;
                        pair = 3;
                    }
                    else if ((long unsigned int)j == line.length() - 2)
                    {
                        passed = true;
                        pair = 3;
                    }

                    if (passed)
                    {
                        move(y + i, x + j - buf.length());
                        printw("%s", buf.c_str());
                        buf = "";
                        attron(COLOR_PAIR(pair));
                    }
                    else
                    {
                        buf += line[j];
                    }
                }

                if (passed)
                {
                    move(y + i, x + j);
                    printw("%c", line[j]);
                    attroff(COLOR_PAIR(pair));
                }
            }

            i++;
        }
    }

    attroff(COLOR_PAIR(1));
}

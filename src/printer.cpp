#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include <string>

#include "Printer.h"
#include "AsciiBuffer.h"
#include "Logger.h"

using namespace std;


string windowTooSmallMessage = "It's a little bit claustrophobic in here...";

int sleeptimeMS = 40;
int offsetX = 0;
int offsetY = 0;

Printer::Color fgColor = Printer::NONE;
Printer::Color bgColor = Printer::NONE;

const vector<int> Printer::ColorRange = {-1, 7}; 

bool autocenter = true;

void Printer::Init(const int setSleeptimeMS, const int setOffsetX, const int setOffsetY)
{
    sleeptimeMS = setSleeptimeMS;
    offsetX = setOffsetX;
    offsetY = setOffsetY;
    
    autocenter = (offsetX == 0 && offsetY == 0);

    setlocale(LC_ALL, "C.UTF-8");
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    start_color();
    use_default_colors();
    curs_set(0);

    Logger::SetNCursesMode(true);
}

void Printer::Stop()
{
    Logger::PrintDebug("Stopping ncurses.");
    clear();
    curs_set(1);
    reset_prog_mode();
    endwin();

    Logger::SetNCursesMode(false);
}

// SETTERS
void Printer::SetDefaultColors(const Printer::Color fg, const Printer::Color bg)
{
    fgColor = fg;
    bgColor = bg;
}

void Printer::SetDefaultForegroundColor(const Color color)
{
    fgColor = color;
}

void Printer::SetDefaultBackgroundColor(const Color color)
{
    bgColor = color;
}


// UTILITY
bool Printer::IsValidColorID(const int colorID)
{
    if (colorID >= ColorRange[0] && colorID <= ColorRange[1])
        return true;
    return false;
}

// PRINTING FUNCTIONS
void Printer::Print(AsciiBuffer &buffer, const bool chromaticAberration)
{
    init_pair(1, fgColor, bgColor);
    int maxX, maxY;
    vector<string>* lines = buffer.GetDistortedLinesPtr();

    getmaxyx(stdscr, maxY, maxX);

    if (autocenter)
    {
        offsetX = 0;
        offsetY = 0;
    }

    if (buffer.GetMaxDistortedLineLength() > maxX - offsetX || int(lines->size()) > maxY - offsetY)
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

    if (autocenter)
    {
        x = 0.5 * (maxX - buffer.GetMaxDistortedLineLength());
        y = 0.5 * (maxY - lines->size());
    }

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

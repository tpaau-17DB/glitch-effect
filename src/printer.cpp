#include <vector>
#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include <string>

#include "Printer.h"
#include "AsciiBuffer.h"
#include "Logger.h"

using namespace std;


string Printer::windowTooSmallMessage = "It's a little bit claustrophobic in here...";

int Printer::sleeptimeMS = 40;
int Printer::offsetX = 0;
int Printer::offsetY = 0;

bool Printer::autocenter = true;

void Printer::Init(const int sleeptimeMS, const int offsetX, const int offsetY)
{
    Printer::sleeptimeMS = sleeptimeMS;
    Printer::offsetX = offsetX;
    Printer::offsetY = offsetY;
    
    autocenter = (offsetX == 0 && offsetY == 0);

    setlocale(LC_ALL, "C.UTF-8");
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
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
void Printer::SetColors(const Printer::Color fg, const Printer::Color bg)
{
    init_pair(1, fg, bg);
}


// PRINTING FUNCTIONS
void Printer::Print(AsciiBuffer &buffer, const bool chromaticAberration)
{
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

    int i = 0;
    int x = offsetX;
    int y = offsetY;
    if (!chromaticAberration)
    {
        for (const string &line : *lines)
        {
            if (autocenter)
            {
                x = 0.5 * (maxX - buffer.GetMaxDistortedLineLength());
                y = 0.5 * (maxY - lines->size());
            }

            move(y + i, x);
            printw("%s", line.c_str());
            i++;
        }
    }
    else
    {
        init_pair(2, COLOR_RED, NONE);
        init_pair(3, COLOR_BLUE, NONE);
        int size;
        int pair;
        bool passed;
        for (const string &line : *lines)
        {
            if (autocenter)
            {
                x = 0.5 * (maxX - buffer.GetMaxDistortedLineLength());
                y = 0.5 * (maxY - lines->size());
            }

            size = line.size() - 1;
            for (int j = 0; j < size; j++)
            {
                passed = false;
                if (line[j] != ' ')
                {
                    if ((j > 0 && line[j - 1] == ' '))
                    {
                        passed = true;
                        pair = 2;
                    }
                    else if (j < size && line [j + 1] == ' ') 
                    {
                        passed = true;
                        pair = 3;
                    }

                    if (passed)
                    {
                        attron(COLOR_PAIR(pair));
                    }
                }

                move(y + i, x + j);
                printw("%c", line[j]);

                if (passed)
                {
                    attroff(COLOR_PAIR(pair));
                }
            }

            i++;
        }
    }

    attroff(COLOR_PAIR(1));
}

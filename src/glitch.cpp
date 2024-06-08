//#include "ConfigLoader.h"
#include "Logger.h"
#include "FileLoader.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <ncurses.h>

using namespace std;

int hashStr(const char *str)
{
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * 31 + *str;
        str++;
    }
    return hash;
}

int main(int argc, char *argv[])
{
    srand(time(nullptr));

    string filepath = "";
    string config_path = "";

    int sleeptime_ms = 40;
    int glitch_strenght = 6;
    int glitch_intensity = 35;
    int offsetx = 0;
    int offsety = 0;
    int fileX = 0;

    bool autocenter = true;
    bool fileSpecified = false;

    for (int i = 1; i < argc; i++)
    {
        unsigned int hash = hashStr(argv[i]);
	//cout<<hash<<endl;

        switch (hash)
        {
        case 1333069025: //  --help
            Logger::PrintUsage();
            return 0;
            break;

        case 1511: //  -t
            if (i + 1 < argc)
            {
                cout << "-t set to: " << argv[i + 1] << endl;
                sleeptime_ms = stoi(argv[i + 1]);
                i++;
            }
            else
            {
                Logger::PrintUnsupported(argv[i]);
                return 1;
            }
            break;

        case 1510: //  -s
            if (i + 1 < argc)
            {
                cout << "-s set to: " << argv[i + 1] << endl;
                glitch_strenght = stoi(argv[i + 1]);
                i++;
            }
            else
            {
                Logger::PrintUnsupported(argv[i]);
                return 1;
            }
            break;

        case 1500: //  -i
            if (i + 1 < argc)
            {
                cout << "-i set to: " << argv[i + 1] << endl;
                glitch_intensity = stoi(argv[i + 1]);
                i++;
            }
            else
            {
                Logger::PrintUnsupported(argv[i]);
                return 1;
            }
            break;

        case 2316757092: //  --autocenter
            if (offsetx > 0 || offsety > 0)
            {
                cout << "You can't enable autocenter, manual offset is anabled!" << endl;
                Logger::PrintUsage();

                return 1;
            }
            autocenter = true;
            cout << "Autocenter enabled" << endl;
            break;

        case 46806: //  -ox
            if (autocenter == true)
            {
                Logger::PrintAutocenterWarn();
                return 1;
            }

            if (i + 1 < argc)
            {
                offsetx = stoi(argv[i + 1]);
                i++;
                break;
            }
            else
            {
                cout << "Unsupported use of an argument: " << argv[i] << endl;
                Logger::PrintUsage();
                return 1;
            }
            break;

        case 46807: //  -oy
            if (autocenter == true)
            {
                Logger::PrintAutocenterWarn();
                return 1;
            }

            if (i + 1 < argc)
            {
                offsety = stoi(argv[i + 1]);
                i++;
                break;
            }
            else
            {
                cout << "Unsupported use of an argument: " << argv[i] << endl;
                Logger::PrintUsage();
                return 1;
            }
            break;
	
	case 1494: // -c
	    if (i + 1 < argc)
            {
                cout << "specified config path: " << argv[i + 1] << endl;
        	config_path = argv[i + 1];
                i++;
            }
            else
            {
                Logger::PrintUnsupported(argv[i]);
                return 1;
            }
	    break;

        default:
            fileSpecified = true;
            filepath = argv[i];
            break;
        }
    }

    if (!fileSpecified)
    {
        cout << "error: no input file!" << endl;
        return 1;
    }

    setlocale(LC_ALL, "C.UTF-8");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    const vector<string> lines = FileLoader::GetLines(filepath);

    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);

    for (const string &str : lines)
    {
        if (static_cast<int>(str.length()) > fileX)
            fileX = str.length();
    }

    if (fileX + (2 * glitch_strenght) > maxX)
    {
        printw("Terminal window is too small for this configuration, continue anyway? (y/N)");
        move(1, 0);

        int response = getch();

        if (response != 'y' && response != 'Y')
        {
            endwin();
            return 1;
        }
    }

    while (true)
    {
        getmaxyx(stdscr, maxY, maxX);

        clear();
        int i = 0;
        for (const string &str : lines)
        {
            int num = 0;

            if ((rand() % glitch_intensity + 1) == 1)
            {
                num = rand() % glitch_strenght + 1;
            }

            int rev_effect = rand() % 2;

            if (autocenter)
            {
                offsetx = 0.5 * (maxX - (fileX + 2 * glitch_strenght));
                offsety = 0.5 * (maxY - lines.size());
            }

            if (rev_effect == 1)
                move(offsety + i, offsetx - num + glitch_strenght);
            else
                move(offsety + i, offsetx + num + glitch_strenght);

            printw(str.c_str());

            i++;
        }

        refresh();

        usleep(1000 * sleeptime_ms);
    }

    endwin();
    return 0;
}

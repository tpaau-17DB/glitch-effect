#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <ncurses.h>

#include "Logger.h"
#include "FileLoader.h"
#include "ConfigLoader.h"

int* Logger::verbosity = new int(1);

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

int getParamFromConf(map<string, int>* conf, const string& param) 
{
    auto it = conf->find(param); 

    if (it != conf->end()) 
    {
        Logger::PrintLog("Param '" + param + "' = " + to_string(it->second));
        return it->second;     
    } 
    else 
    {
        Logger::PrintWarn("Param '" + param + "' not found in config file.");
        return -1;     
    }
}

int main(int argc, char *argv[])
{
    srand(time(nullptr));

    string filepath = "";
    string config_path = "../config/glitch-effect.conf";

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

        case 46806: //  -ox, offset X
            if (i + 1 < argc)
            {
                offsetx = stoi(argv[i + 1]);

		Logger::PrintLog("offset x set to: " + string(argv[i + 1]));

		if(autocenter) Logger::PrintLog("autocenter disabled");
		autocenter = false;

                i++;
                break;
            }
            else
            {
		Logger::PrintUnsupported(argv[i]);
                return 1;
            }
            break;

        case 46807: //  -oy, offset Y
            if (i + 1 < argc)
            {
                offsety = stoi(argv[i + 1]);

		Logger::PrintLog("offset y set to: " + string(argv[i + 1]));

		if(autocenter) Logger::PrintLog("autocenter disabled");
		autocenter = false;

                i++;
                break;
            }
            else
            {
                Logger::PrintUnsupported(argv[i]);
                return 1;
            }
            break;
	
	case 1494: // -c, config
	    if (i + 1 < argc)
            {
		config_path = argv[i + 1];
		Logger::PrintLog("specified config path: " + string(argv[i + 1]));
                i++;
		break;
            }
	    else
            {
                Logger::PrintUnsupported(argv[i]);
                return 1;
            }
	    break;

	case 1513: // -v, verbosity
	    if (i + 1 < argc)
            {
		Logger::SetVerbosity(stoi(argv[i + 1]));
		Logger::PrintLog("verbosity set to: " + string(argv[i + 1]));
                i++;
		break;
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
	Logger::PrintErr("error: no input file!");
        return 1;
    }

    Logger::PrintLog("Attempting to load a config file at '" + config_path + "'");
    map<string, int> config = ConfigLoader::LoadConf(config_path);

    int conf_exit_code = getParamFromConf(&config, "exit-code");

    int* output_tmp;

    if (conf_exit_code == 0)
    {
	Logger::PrintLog("Config loaded succesfully.");

	output_tmp = new int(getParamFromConf(&config, "verbosity"));
    	if (*output_tmp != -1)
    	    Logger::SetVerbosity(*output_tmp);
    	delete output_tmp;

    	output_tmp = new int(getParamFromConf(&config, "strenght"));
    	if (*output_tmp != -1)
    	    glitch_strenght = *output_tmp;
    	delete output_tmp;

    	output_tmp = new int(getParamFromConf(&config, "intensity"));
    	if (*output_tmp != -1)
    	    glitch_intensity = *output_tmp;
    	delete output_tmp;

	output_tmp = new int(getParamFromConf(&config, "sleeptime"));
    	if (*output_tmp != -1)
    	    sleeptime_ms = *output_tmp;
    	delete output_tmp;
    }
    else
    {
	Logger::PrintErr("Failed to load config file! Function returned '" + to_string(conf_exit_code) + "'!");
    }

    setlocale(LC_ALL, "C.UTF-8");
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    const vector<string> lines = FileLoader::GetLines(filepath);

    for (const string &str : lines)
    {
        if (static_cast<int>(str.length()) > fileX)
            fileX = str.length();
    }

    int maxX, maxY;

    while (true)
    {
        getmaxyx(stdscr, maxY, maxX);

        if (fileX + (2 * glitch_strenght) > maxX)
        {
            clear();
            move(1, 0);
            printw("Terminal window is too small. I'll just wait...");
	    refresh();
	    usleep(200000);
	    continue;
        }



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

            printw("%s", str.c_str());

            i++;
        }

	int ch = getch();
	if (ch == 'q')
            break; 

        refresh();

        usleep(1000 * sleeptime_ms);
    }

    endwin();
    return 0;
}

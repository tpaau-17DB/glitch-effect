#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <stdexcept>
#include <ncurses.h>
#include <cstring>

using namespace std;

void printUsage()
{
  cout<<"Usage: glitch [options] file.txt"<<endl;
  cout<<"  --help: Display help message"<<endl;
  cout<<"  -t <number>: Set maximum delay in miliseconds (default: 100 )"<<endl;
  cout<<"  -s <number>: Set effect strenght (default: 10, max line deformation strenght)"<<endl;
  cout<<"  -i <number>: Set effect intensity (default: 20, one line in <n> gets deformed)"<<endl;
  cout<<"  --autocenter: Enable autocenter"<<endl;
  cout<<"  -ox <number>: Manually set X offset"<<endl;
  cout<<"  -oy <number>: Manually set Y offset"<<endl;
}

void printUnsupported(const char *arg)
{
  cout<<"Unsupported use of an argument: "<<arg<<endl;
  printUsage();
}

int hashStr(const char *str)
{
  unsigned int hash = 0;
  while(*str)
  {
    hash = hash * 31 + *str;
    str++;
  }
  return hash;
}

vector<string> getLines(const string& path)
{
  vector<string> lines;
  ifstream file(path);

  if(!file.good())
  {
    throw runtime_error("File error! File path: '" + path + "'");
    //printUsage();
  }

  if(file.is_open())
  {
    string line;
    
    while(getline(file, line))
    {
      lines.push_back(line);
    }

    file.close();
  }
  else
  {
    cerr<<"Unable to open file: "<<path<<endl;
    printUsage();
  }

  return lines;
}

void print_autocenter_warn()
{
  cout<<"You can't manually set offset, bacouse autocenter is alredy enabled!"<<endl;
  printUsage();
}

int main(int argc, char* argv[])
{
  srand(time(nullptr));

  string filepath = "text.txt";
  const char spc = ' ';

  unsigned int sleeptime_ms = 100;
  unsigned int glitch_strenght = 8;
  unsigned int glitch_intensity = 20;
  unsigned int offsetx = 0;
  unsigned int offsety = 0;
  unsigned int fileX = 0;

  bool autocenter = false;
  bool fileSpecified = false;

  for (int i = 1; i < argc; i++)
  {
    unsigned int hash = hashStr(argv[i]);

    //cout<<hash<<endl;

    switch(hash)
    {
      case 1333069025: //  --help
	printUsage();
	return 0;
	break;

      case 1511: //  -t
        if(i + 1 < argc)
        {
	  cout<<"-t set to: "<<argv[i + 1]<<endl;
  	  sleeptime_ms = 1000 * stoi(argv[i + 1]);
  	  i++;
        }
        else
        {
	  printUnsupported(argv[i]);
	  return 1;
        }
	break;

      case 1510: //  -s
	if(i + 1 < argc)
        {
	  cout<<"-s set to: "<<argv[i + 1]<<endl;
  	  glitch_strenght = stoi(argv[i + 1]);
  	  i++;
        }
        else
        {
	  printUnsupported(argv[i]);
	  return 1;
        }
	break;

      case 1500: //  -i
	if(i + 1 < argc)
        {
	  cout<<"-i set to: "<<argv[i + 1]<<endl;
  	  glitch_intensity = stoi(argv[i + 1]);
  	  i++;
        }
        else
        {
	  printUnsupported(argv[i]);
	  return 1;
        }
	break;

      case 2316757092: //  --autocenter
        if(offsetx > 0 || offsety > 0)
        {
          cout<<"You can't enable autocenter, becouse manual offset is anabled!"<<endl;
          printUsage();

          return 1;
        }
        autocenter = true;
	cout<<"Autocenter enabled"<<endl;
	break;

      case 46806: //  -ox
        if(autocenter == true)
        {
  	  print_autocenter_warn();
  	  return 1;
        }

        if(i + 1 < argc)
        {
	  offsetx = stoi(argv[i + 1]);
	  i++;
	  break;
        }
        else
        {
	  cout<<"Unsupported use of an argument: "<<argv[i]<<endl;
	  printUsage();
	  return 1;
        }
	break;

      case 46807: //  -oy
        if(autocenter == true)
        {
  	  print_autocenter_warn();
  	  return 1;
        }

        if(i + 1 < argc)
        {
	  offsety = stoi(argv[i + 1]);
	  i++;
	  break;
        }
        else
        {
	  cout<<"Unsupported use of an argument: "<<argv[i]<<endl;
	  printUsage();
	  return 1;
        }
	break;

      default:
        fileSpecified = true;
        filepath = argv[i];
	break;
    }
  }

  if(!fileSpecified)
  {
    cout<<"error: no input file!"<<endl;
    return 1;
  }

  setlocale(LC_ALL, "C.UTF-8");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  const vector<string> lines = getLines(filepath);

  int maxX, maxY;
  int response = 0;
  getmaxyx(stdscr, maxY, maxX);

  for(const string& str : lines)
  {
    if(str.length() > fileX) fileX = str.length();
  }

  if(fileX + (2 * glitch_strenght) > maxX)
  {
    printw("Terminal window is too small for this configuration, continue anyway? (y/N)");
    move(1, 0);

    response = getch();

    if(response != 'y' && response != 'Y')
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
    for (const string& str : lines)
    {
      int num = 0;

      if((rand() % glitch_intensity + 1) == 1)
      {
	num = rand() % glitch_strenght + 1;
      }

      int rev_effect = rand() % 2;

      if(autocenter)
      {
	offsetx = 0.5 * (maxX - (fileX + 2 * glitch_strenght));
	offsety = 0.5 * (maxY - lines.size());
      }

      if(rev_effect == 1)
        move(offsety + i, offsetx - num + glitch_strenght);
      else
        move(offsety + i, offsetx + num + glitch_strenght);

      printw(str.c_str());

      i++;
    }

    refresh();

    int sleeptime = rand() % sleeptime_ms + 1;

    usleep(sleeptime);
    //system("clear");
  }

  endwin();
  return 0;
}

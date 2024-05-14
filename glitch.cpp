#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <stdexcept>
#include <ncurses.h>

using namespace std;

void printUsage()
{
  cout<<"Usage: fuck you"<<endl;
}

vector<string> getLines(const string& path)
{
  vector<string> lines;
  ifstream file(path);

  if(!file.good())
  {
    throw runtime_error("File not found! File path: '" + path + "'");
    printUsage();
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

  unsigned int sleeptime_ms = 30000;
  unsigned int glitch_strenght = 3;
  unsigned int glitch_intensity = 5;
  unsigned int offsetx = 0;
  unsigned int offsety = 0;
  unsigned int fileX = 0;

  bool autocenter = false;

  //cout<<"argc: "<<argc<<endl;
  for (int i = 1; i < argc; i++)
  {
    //cout<<"arg: "<<argv[i]<<endl;

    if (strcmp(argv[i], "-t") == 0)
    {
      if(i + 1 < argc)
      {
	sleeptime_ms = 1000 * stoi(argv[i + 1]);
	i++;
      }
      else
      {
	cout<<"Unsupported use of an argument: "<<argv[i]<<endl;
	printUsage();
	return 1;
      }

      continue;
    }
    if(strcmp(argv[i], "-s") == 0)
    {
      if(i + 1 < argc)
      {
	glitch_strenght = stoi(argv[i + 1]);
	i++;
      }
      else
      {
        cout<<"Unsupported use of an argument: "<<argv[i]<<endl;
        printUsage();
        return 1;
      }

      continue;
    }
    if(strcmp(argv[i], "-i") == 0)
    {
      if(i + 1 < argc)
      {
	glitch_intensity = stoi(argv[i + 1]);
	i++;
	continue;
      }
      else
      {
	cout<<"Unsupported use of an argument: "<<argv[i]<<endl;
	printUsage();
	return 1;
      }

      continue;
    }
    if(strcmp(argv[i], "--autocenter") == 0)
    {
      if(offsetx > 0 || offsety > 0)
      {
        cout<<"You can't enable autocenter, becouse manual offset is anabled!"<<endl;
        printUsage();

	return 1;
      }

      autocenter = true;
      continue;
    }
    if(strcmp(argv[i], "-ox") == 0)
    {
      if(autocenter == true)
      {
	print_autocenter_warn();
	return 1;
      }

      if(i + 1 < argc)
      {
	offsetx = stoi(argv[i + 1]);
	i++;
	continue;
      }
      else
      {
	cout<<"Unsupported use of an argument: "<<argv[i]<<endl;
	printUsage();
	return 1;
      }

      continue;
    } 
    if(strcmp(argv[i], "-oy") == 0)
    {
      if(autocenter == true)
      {
	print_autocenter_warn();
	return 1;
      }

      if(i + 1 < argc)
      {
	offsety = stoi(argv[i + 1]);
	i++;
	continue;
      }
      else
      {
	cout<<"Unsupported use of an argument: "<<argv[i]<<endl;
	printUsage();
	return 1;
      }

      continue;
    }
    else
    {
      filepath = argv[i];
    }
    
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

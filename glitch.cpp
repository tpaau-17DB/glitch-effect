#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <stdexcept>

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
    if(strcmp(argv[i], "-ox") == 0)
    {
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
 
  const vector<string> lines = getLines(filepath);

  while (true)
  {
    int i = 0;
    cout<<(string(offsety, '\n'));
    for (const string& str : lines)
    {
      int num = 0;

      if((rand() % glitch_intensity + 1) == 1)
      {
	num = rand() % glitch_strenght + 1;
      }

      int rev_effect = rand() % 2;
      string space = "";

      if(rev_effect == 1)
	space = string(glitch_strenght - num, spc);
      else
        space = string(glitch_strenght + num, spc);

      cout<<string(offsetx, spc) << space << str << endl;

      i++;
    }

    int sleeptime = rand() % sleeptime_ms + 1;

    usleep(sleeptime);
    system("clear");
  }

  return 0;
}

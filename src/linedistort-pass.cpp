#include "Linedistort-pass.h"

#include <vector>
#include <string>

using namespace std;

void Linedistort_pass::distort_pass(const vector<string>* lines)
{
           for (const string &str : *lines)
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
}

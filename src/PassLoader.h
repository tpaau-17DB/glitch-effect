#ifndef PASS_LOADER_H
#define PASS_LOADER_H

#include <vector>

#include "Logger.h"



class PassLoader
{
    public:
        enum passType
        {
            VerticalDistort,
        };

        struct passParams
        {
            int Strength = 8;
            int Intensity = 30;

            bool RevCol = false;
        };

        struct pass
        {
            passType PassType;
            passParams PassParams;
        };

        static int GetPassesFromJSON(const std::string& path, std::vector<PassLoader::pass>&);
};

#endif

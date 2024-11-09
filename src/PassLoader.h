#ifndef PASS_LOADER_H
#define PASS_LOADER_H

#include <vector>

#include "Logger.h"

class PassLoader
{
    public:
        enum PassType
        {
            VerticalDistort,
            Undefined,
        };

        struct PassParams
        {
            int Strength = 8;
            int Intensity = 30;

            bool RevCol = false;
        };

        struct pass
        {
            PassType PT;
            PassParams PP;
        };

        static std::vector<PassLoader::pass> GetPassesFromJSON(const std::string& path);
};

#endif

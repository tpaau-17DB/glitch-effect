#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <vector>

#include "Logger.h"

class ConfigLoader
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

        struct GlobalConfig
        {
            // METADATA
            bool loadedCorrectly = false;

            // VARIABLES
            int SleeptimeMS = 40;
            unsigned short LoggerVerbosity = 1;
        };

        static std::vector<ConfigLoader::pass> GetPassesFromJSON(const std::string& path);
        static ConfigLoader::GlobalConfig GetGlobalConfig(const std::string& path);
};

#endif

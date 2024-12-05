#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <vector>

#include "Logger.h"

class ConfigLoader
{
    public:
        enum PassType
        {
            CharacterShuffle,
            HorizontalDistort,
            Discard,
            Undefined,
        };

        struct pass
        {
             PassType Type;
             int Strength = 8;
             int Intensity = 30;

             bool RevCol = false;
        };

        struct GlobalConfig
        {
            // METADATA
            bool LoadedCorrectly = false;

            // VARIABLES
            // "///notspecified///" is the default value, that indicates that
            // default ascii path line was not present in the config file.
            std::string DefaultAsciiPath = "///notspecified///";
            int SleeptimeMS = 40;
            unsigned short LoggerVerbosity = 1;
        };

        static std::vector<ConfigLoader::pass> GetPassesFromJSON(std::string& path);
        static ConfigLoader::GlobalConfig GetGlobalConfig(std::string& path);
};

#endif

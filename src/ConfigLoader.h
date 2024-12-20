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
             PassType Type = Undefined;
             int Strength = 8;
             int Intensity = 30;

             bool ReverseColor = false;
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

            std::string foregroundColorName = "NONE";
            std::string backgroundColorName = "NONE";

            bool UseChromaticAberration = false;
        };

        static std::vector<pass> GetPassesFromJSON(std::string& path);
        static GlobalConfig GetGlobalConfig(std::string& path);
};

#endif

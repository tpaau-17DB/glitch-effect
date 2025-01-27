#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <vector>

#include "Logger.h"

enum PassType
{
    CHARACTER_SHUFFLE,
    HORIZONTAL_DISTORT,
    DISCARD,
    HORIZONTAL_OFFSET,
    UNDEFINED,
};

struct Pass
{
     PassType type = UNDEFINED;
     int strength = 8;
     int intensity = 30;
     bool ignoreSpaces = true;
};

struct GlobalConfig
{
    // METADATA
    bool loadedCorrectly = false;

    // VARIABLES
    // default ascii path specified in the config file.
    std::string defaultAsciiPath = "";

    int sleeptimeMS = 40;

    unsigned short loggerVerbosity = 1;

    std::string foregroundColorStr = "NONE";
    std::string backgroundColorStr = "NONE";

    bool chromaticAberrationEnabled = false;
};

std::vector<Pass> getPassesFromJSON(std::string& path);
GlobalConfig getGlobalConfig(std::string& path);

#endif

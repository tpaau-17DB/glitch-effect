#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <random>
#include <vector>

#include "ConfigLoader.h"

// this class holds common functions
class Utils
{
    public:
        static int StrToInt(const std::string& str);
        static int StrToColorID(const std::string& str);
        static unsigned long HashString(const std::string& str);
        static float GetRandomFloat(const float min, const float max);
        static ConfigLoader::PassType GetPassTypeFromName(const std::string& name);
        static std::string RemoveComments(const std::string& str);

    private:
        static std::random_device rd;
};

#endif

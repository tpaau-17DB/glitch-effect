#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <random>

class Utils
{
    public:
        static float GetRandomFloat(const float min, const float max);
        static int StrToInt(const std::string& str);
        static int StrToColorID(const std::string& str);
        static unsigned long HashString(const std::string& str);
        static std::string RemoveComments(const std::string& str);

    private:
        static std::random_device rd;
};

#endif

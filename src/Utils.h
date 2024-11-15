#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils
{
    public:
        static int StrToInt(const std::string& str);
        static int StrToColorID(const std::string& str);
        static unsigned long HashString(const std::string& str);
};

#endif

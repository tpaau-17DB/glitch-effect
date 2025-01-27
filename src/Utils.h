#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <random>
#include <vector>

#include "ConfigLoader.h"

int strToInt(const std::string& str);
int strToColorID(const std::string& str);

float getRandomFloat(const float min, const float max);
int getRandomInt(const int min, const int max);
int getRandomPrecalculatedInt();

std::vector<std::string> removeANSICodes(const std::vector<std::string>& lines);

std::string toLower(std::string str);

#endif

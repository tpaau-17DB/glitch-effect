#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::string getEnvVar(const std::string& var);
std::vector<std::string> getLines(std::string &path);
bool checkIfFileExists(const std::string &path);
std::string lookForConfigFile();
std::string expandPath(const std::string &path);

#endif

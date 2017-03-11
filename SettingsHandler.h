#pragma once
#include <iostream>
#include <fstream>
#include <string>

void settingsSaver(std::string outputFile, std::string* settings, int strLen);
void settingsLoader(std::string inputFile, std::string* settings);
void settingsBuilder(std::string* settings, std::string message);
std::string ConvertCLIPointerToStdString(array<unsigned char>^ s);
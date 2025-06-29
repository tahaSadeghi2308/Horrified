/*
 * @brief Here we implement some useful and non relavent functions or classes to main game classes
*/

#include <string>

#ifndef UTILITY_HPP
#define UTILITY_HPP

// -------- Functions -------------------
void clearScreen();
int getCommand(std::string prompt = "");
bool isNumber(std::string sequence);
#endif // UTILITY_HPP
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

void clearScreen();

int getCommand(std::string prompt = "");

bool isNumber(std::string sequence);

#endif // UTILITY_HPP
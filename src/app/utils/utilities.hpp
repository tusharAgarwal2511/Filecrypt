#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// ANSI colors
inline const std::string RESET = "\033[0m";
inline const std::string RED = "\033[31m";
inline const std::string GREEN = "\033[32m";
inline const std::string YELLOW = "\033[33m";
inline const std::string CYAN = "\033[36m";
inline const std::string BOLD = "\033[1m";

// Terminal & UI functions
void moveCursorUp(int n);
char getKey();
int showMenu(const std::vector<std::string> &options);
void printHeader();
void showProgressBar(size_t current, size_t total);
void printSummary(size_t files, double mbProcessed, double duration, double speed);

#endif
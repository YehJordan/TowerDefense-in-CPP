#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

// Global file stream object for logging
extern std::ofstream logFile;

// Function to log observer statements to the file
void logObserver(const std::string& message);

#endif
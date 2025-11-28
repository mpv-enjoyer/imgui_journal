#pragma once
#include <iostream>

inline void log_warning(std::string reason)
{
    std::cerr << "[WARNING] " << reason << "\n";
}

inline void log_error(std::string reason)
{
    std::cerr << "[ERROR] " << reason << "\n";
}
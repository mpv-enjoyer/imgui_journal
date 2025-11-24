#pragma once
#include <iostream>

void log_warning(std::string reason)
{
    std::cerr << "[WARNING] " << reason << "\n";
}

void log_error(std::string reason)
{
    std::cerr << "[ERROR] " << reason << "\n";
}
#pragma once
#include <optional>
#include <string>

// Rotate backup files. Removes backups that are beyond [ext_begin, ext_end]:
std::optional<std::string> update_backups(const std::string& file_name, char ext_begin, char ext_end);

// Removes backups that are beyond [ext_begin, ext_end]:
std::optional<std::string> remove_beyond_backups(const std::string& file_name, char ext_begin, char ext_end);
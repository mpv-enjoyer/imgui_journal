#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <optional>
#include <chrono>
#include "backup.h"

// debug_output.hpp
#include <cstdarg>
#include <cstdio>
#include <cassert>
#include <iostream>
#if __cplusplus >= 202302L
 #include <format>
 #include <print>
#endif // __cplusplus >= 202302L

int debug_printf(const char* fmt, ...);
int debug_puts(const char* str);
int debug_fputs(const char* str, FILE* stream);
int debug_putc(int ch, FILE* stream);
int debug_fputc(int ch, FILE* stream);
#if __cplusplus >= 202302L
 template <class... Args>
 void debug_println(std::format_string<Args...> fmt, Args&&... args);
 template <class... Args>
 void debug_println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args);
 template <class... Args>
 void debug_print(std::format_string<Args...> fmt, Args&&... args);
 template <class... Args>
 void debug_print(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args);
#endif // __cplusplus >= 202302L

#ifdef DO_DEBUG
 int debug_printf(const char* fmt, ...)
 {
     va_list arg;
     va_start(arg, fmt);
     int result = vprintf(fmt, arg);
     va_end(arg);
     return result;
 }
 int debug_puts(const char* str)
 {
     return puts(str);
 }
 int debug_fputs(const char* str, FILE* stream)
 {
     return fputs(str, stream);
 }
 int debug_putc(int ch, FILE* stream)
 {
     return putc(ch, stream);
 }
 int debug_fputc(int ch, FILE* stream)
 {
     return fputc(ch, stream);
 }
 #define debug_assert(x) assert(x)

 #define DEBUG_PRINTF debug_printf
 #define DEBUG_PUTS debug_puts
 #define DEBUG_FPUTS debug_fputs
 #define DEBUG_PUTC debug_putc
 #define DEBUG_FPUTC debug_fputc
 #define DEBUG_ASSERT assert
 
 #if __cplusplus >= 202302L
  template <class... Args>
  void debug_println(std::format_string<Args...> fmt, Args&&... args)
  {
   std::println(fmt, std::forward<Args>(args)...);
  }
  template <class... Args>
  void debug_println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
  {
   std::println(stream, fmt, std::forward<Args>(args)...);
  }
  template <class... Args>
  void debug_print(std::format_string<Args...> fmt, Args&&... args)
  {
   std::print(fmt, std::forward<Args>(args)...);
  }
  template <class... Args>
  void debug_print(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
  {
   std::print(stream, fmt, std::forward<Args>(args)...);
  }

  #define DEBUG_PRINT debug_print
  #define DEBUG_PRINTLN debug_println
 #endif // __cplusplus >= 202302L

 std::ostream& debug_cout = std::cout;

#else // DO_DEBUG

 int debug_printf(const char* fmt, ...)
 {
     return 0;
 }
 int debug_puts(const char* str)
 {
     return 0;
 }
 int debug_fputs(const char* str, FILE* stream)
 {
     return 0;
 }
 int debug_putc(int ch, FILE* stream)
 {
     return ch;
 }
 int debug_fputc(int ch, FILE* stream)
 {
     return ch;
 }
 #define debug_assert(x) do {(x);} while (0)

 #define DEBUG_PRINTF(...) do {} while (0)
 #define DEBUG_PUTS(...) do {} while (0)
 #define DEBUG_FPUTS(...) do {} while (0)
 #define DEBUG_PUTC(...) do {} while (0)
 #define DEBUG_FPUTC(...) do {} while (0)
 #define DEBUG_ASSERT(...) do {} while (0)

 #if __cplusplus >= 202302L
  template <class... Args>
  void debug_println(std::format_string<Args...> fmt, Args&&... args)
  { }
  template <class... Args>
  void debug_println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
  { }
  template <class... Args>
  void debug_print(std::format_string<Args...> fmt, Args&&... args)
  { }
  template <class... Args>
  void debug_print(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
  { }
  #define DEBUG_PRINTLN(...) do {} while (0)
  #define DEBUG_PRINT(...) do {} while (0)
 #endif // __cplusplus >= 202302L

 // Thanks, Ulrich Eckhardt: https://groups.google.com/g/comp.lang.c++.moderated/c/ggl_2Ii3aVM/m/r3slzSfZ6XkJ
 std::ostream debug_cout(0);

#endif // DO_DEBUG
// debug_output.hpp

struct Parsed
{
    std::filesystem::path path;
    char ext;
    std::filesystem::file_time_type time;
    bool operator<(Parsed& other) const
    {
        return ext < other.ext;
    }
};

static bool starts_with(std::string str, std::string prefix)
{
    return str.rfind(prefix, 0) == 0; // https://stackoverflow.com/a/40441240
}

[[maybe_unused]] static bool ends_with(std::string str, std::string postfix)
{
    if (str.size() < postfix.size()) return false;
    std::size_t start = str.size() - postfix.size();
    return str.find(postfix, start) == start;
}

static void update_backups_may_throw(const std::string& arg_file_name, char ext_begin, char ext_end)
{
    std::filesystem::path arg_file_path(arg_file_name);
    if (!std::filesystem::exists(arg_file_path))
    {
        std::cerr << "[ERROR] CANNOT BACKUP " << arg_file_name << ": !std::filesystem::exists(arg_file_path)\n";
        return;
    }
    if (!std::filesystem::is_regular_file(arg_file_path))
    {
        std::cerr << "[ERROR] CANNOT BACKUP " << arg_file_name << ": !std::filesystem::is_regular_file(arg_file_path)\n";
        return;
    }
    // IM_ASSERT(!ends_with(arg_file_name, DELIMITER));
    const static std::string BACKUP_FOLDER = "backup/";
    const static std::string DELIMITER = ".bkp_";
    DEBUG_ASSERT(ext_begin < ext_end);
    const std::string prefix = arg_file_name + DELIMITER;
    std::vector<Parsed> files;
    std::vector<std::filesystem::path> to_remove;
    if (!std::filesystem::exists(std::filesystem::path(BACKUP_FOLDER)))
    {
        std::error_code ec;
        bool result = std::filesystem::create_directory(std::filesystem::path(BACKUP_FOLDER), ec);
        if (!result)
        {
            std::cout << "CANNOT create backup folder " << BACKUP_FOLDER << ": " << ec.message() << "\n";
            return;
        }
        std::cout << "Created backup folder " << BACKUP_FOLDER << "\n";
    }
    for (const auto& entry : std::filesystem::directory_iterator(BACKUP_FOLDER))
    {
        auto filepath = entry.path();
        debug_cout << filepath << "\n";
        if (!entry.is_regular_file())
        {
            std::cerr << filepath.string() << ": !entry.is_regular_file()\n";
            continue;
        }
        if (!filepath.has_filename())
        {
            std::cerr << filepath.string() << ": !entry.path().has_filename()\n";
            continue;
        }
        auto filename = filepath.filename();
        if (!starts_with(filename.string(), arg_file_path.filename()))
        {
            debug_cout << filename.string() << " doesn't start with " << arg_file_path << "\n"; 
            continue;
        }
        if (filename.string().size() != prefix.size() + 1)
        {
            std::cerr << filepath.string() << ": filename.string().size() != prefix.size() + 1\n";
            continue;
        }
        char ext = filename.string().back();
        if (ext < ext_begin || ext > ext_end)
        {
            std::cerr << filepath.string() << ": ext < ext_begin || ext > ext_end\n";
            to_remove.push_back(filepath);
            continue;
        }
        
        auto time = entry.last_write_time();
        files.push_back(Parsed{.path = filepath, .ext = ext, .time = time});
    }
    
    std::sort(files.rbegin(), files.rend());

    // Edit filesystem:
    if (to_remove.size() > 0)
    {
        for (const auto& file : to_remove)
        {
            debug_cout << "Removing " << file.string() << "\n";
            std::filesystem::remove(file);
        }
    }

    if (files.size() > 0)
    {
        bool most_recent_backup_too_recent = std::filesystem::last_write_time(arg_file_path) < files.back().time + std::chrono::hours(24);
        if (most_recent_backup_too_recent)
        {
            debug_cout << "Replacing a most recent backup without ever rotating (did a backup in last 24 hours).\n";
            std::filesystem::copy_file(arg_file_path, files.back().path, std::filesystem::copy_options::overwrite_existing);
            return;
        }
    }

    for (std::size_t i = 0; i < files.size(); i++)
    {
        if (files[i].ext == ext_end)
        {
            std::filesystem::remove(files[i].path);
            debug_cout << "removed\n";
        }
        else
        {
            std::filesystem::path path_new = std::filesystem::path(BACKUP_FOLDER + prefix + char(files[i].ext + 1));
            std::filesystem::rename(files[i].path, path_new);
            debug_cout << "renamed\n";
        }
    }

    std::filesystem::path path_new = std::filesystem::path(BACKUP_FOLDER + prefix + ext_begin);
    std::filesystem::copy_file(arg_file_path, path_new);
    debug_cout << "copied" << arg_file_path << " to " << path_new << "\n";
}

std::optional<std::string> update_backups(const std::string& file_name, char ext_begin, char ext_end)
{
    try
    {
        update_backups_may_throw(file_name, ext_begin, ext_end);
    }
    catch (const std::exception& e)
    {
        std::stringstream s;
        s << "Cannot rotate backups for " << file_name << ": " << e.what() << "\n";
        return s.str();
    }
    return {};
}

#if 0 // example usage:
int main(int argc, char** argv)
{
    if (argc == 2)
    {
        auto result = update_backups(argv[1]);
        if (result)
        {
            printf("ERROR: %s\n", result->c_str());
        }
    }
}
#endif 
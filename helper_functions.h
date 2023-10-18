#pragma once
#include "main.h"

const char* c_str_int(int num);
std::string to_string(JTime value);
std::string to_string(JTime begin, JTime end);
std::string to_string(std::tm day, JTime begin, JTime end = {-1, -1}, bool abbreviate = true);
std::string generate_label(const std::string prefix, std::vector<int> unique);
bool j_button_selectable(const char* label, bool selected, bool small = false);
bool j_input_time(std::string label, JTime& time);
bool j_attendance_combo(const char* label, int* status);
template <typename T = int>
bool is_in_vector(std::vector<T> vector, T to_find);
int calculate_first_mwday(int current_mday, int current_wday);
int get_first_wday(int month, int year, int wday);
int get_number_of_days(int month, int year);
int get_wday(int day, int month, int year);
void HelpMarker(const char* desc);
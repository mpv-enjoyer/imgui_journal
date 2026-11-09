#include "helpers.h"

static std::string TEMP_STR_FOR_INT;

const char* c_str_int(int num)
{
    TEMP_STR_FOR_INT = std::to_string(num);
    return TEMP_STR_FOR_INT.c_str();
}

std::string to_string(JTime value)
{
    std::string output;
    output.append(std::to_string(value.hours));
    output.append(":");
    if (value.minutes < 10) output.append("0");
    output.append(std::to_string(value.minutes));
    return output;
}

std::string to_string(JTime begin, JTime end)
{
    return to_string(begin) + " - " + to_string(end);
}

std::string to_string(std::tm day, JTime begin, JTime end, bool abbreviate)
{
    std::string output;
    output.append(std::to_string(day.tm_mday));
    output.append(" ");
    if (day.tm_mon < 12 && day.tm_mon > -1) output.append(Month_Names[day.tm_mon]);
    output.append(", ");
    JTime empty_end = {-1, -1};
    if (end == empty_end) output.append(to_string(begin));
    else output.append(to_string(begin, end));

    if (day.tm_wday < 7 && day.tm_wday > -1) 
    {
        output.append(", ");
        if (abbreviate) output.append(Day_Names_Abbreviated[day.tm_wday]);
        if (!abbreviate) output.append(Day_Names[day.tm_wday]);
    }
    return output;
}

std::string to_string(const std::vector<std::string>& strings, std::string separator, const std::vector<bool>& show_only)
{
    std::string output = "";
    bool pick_individual = show_only.size() == strings.size();
    for (int i = 0; i < strings.size(); i++)
    {
        if ((pick_individual && show_only[i]) || !pick_individual) 
        {
            auto index = strings[i].find("##");
            if (index != strings[i].npos)
            {
                std::string temp = strings[i].substr(0, index);
                output.append(temp);
            }
            else output.append(strings[i]);
        }
        if (i != strings.size() - 1) output.append(separator);
    }
    return output;
}

int validate_time_int(std::string input_string, int upper_limit)
{
    if (input_string.size() == 0) return -1;
    for (int i = 0; i < input_string.size(); i++)
    {
        if (input_string[i] < '0' || input_string[i] > '9') return -1;
    }
    int output = std::stoi(input_string);
    if (output >= upper_limit) return upper_limit - 1;
    if (output < 0) return 0;
    return output;
}

std::string generate_label(const std::string prefix, std::vector<int> unique)
{
    std::string output = prefix;
    for (int i = 0; i < unique.size(); i++)
    {
        output.append("." + std::to_string(unique[i]));
    }
    return output;
}

template <typename T = int, const Student&>
bool is_in_vector(std::vector<T> vector, T to_find)
{
    for (int i = 0; i < vector.size(); i++)
    {
        if (to_find == vector[i]) return true;
    }
    return false;
};

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
};

int get_number_of_days(int month, int year_starting_from_zero)
{
    month++;
    int year = year_starting_from_zero;
    // leap year condition, if month is 2
    if (month == 2) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    // months which have 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
        || month == 10 || month == 12)
        return 31;
    else
        return 30;
}

int get_first_mwday(int month, int year)
{
    return get_wday(0, month, year);
}

int get_first_wday(int month, int year, int wday)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      1, month, year}; // 1-based day, 0-based month, year since 1900

  std::time_t time_temp = std::mktime(&time_in);

  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm time_out = *std::localtime(&time_temp);
  int first_mday_wday = time_out.tm_wday;
  int diff = ( ( wday - first_mday_wday ) + 7 ) % 7;
  return diff + 1;
}

int get_wday(int day, int month, int year)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      day + 1, month, year }; // 1-based day, 0-based month, year since 1900
  std::time_t time_temp = std::mktime(&time_in);
  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm * time_out = std::localtime(&time_temp);
  return time_out->tm_wday;
}

int get_mday_index_for_wday(int mday, int wday, int month, int year)
{
    int day = get_first_wday(month, year, wday) - MDAY_DIFF;
    int day_count = get_number_of_days(month, year + 1900);
    for (int i = 0; day <= day_count; day += 7, i++)
    {
        if (day == mday) return i;
    }
    return -1;
};

int get_wday_count_in_month(int wday, int month, int year)
{
    int output = 0;
    int day = get_first_wday(month, year, wday);
    int day_count = get_number_of_days(month, year);
    for ( ; day <= day_count; day+=7, output++) { };
    return output;
}

void next_month_for(int& month, int& year)
{
    if (month == 11)
    {
        month = 0;
        year = year + 1;
    }
    else
    {
        month += 1;
    }
}

void previous_month_for(int& month, int& year)
{
    if (month == 0)
    {
        month = 11;
        year = year - 1;
    }
    else
    {
        month -= 1;
    }
}

Time_Archiver::Time_Archiver(std::tm tm)
{
    mday = tm.tm_mday;
    month = tm.tm_mon;
    wday = tm.tm_wday;
    year = tm.tm_yday;
}

std::tm Time_Archiver::to_tm()
{
    std::tm tm;
    tm.tm_mday = mday;
    tm.tm_mon = month;
    tm.tm_wday = wday;
    tm.tm_yday = year;
    return tm;
}
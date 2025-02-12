#pragma once
#include "../main.h"

struct JTime //used separately with ctime.
{
    int hours; //0-23
    int minutes; //0-59
};

struct Group_Pair
{
    int number;
    int age_group;
};

struct Lesson_Pair
{
    JTime time_begin;
    JTime time_end;
    int lesson_name_id;
};

struct Lesson
{
    int merged_lesson_id;
    int internal_lesson_id;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
    ar & merged_lesson_id;
    ar & internal_lesson_id;
}
};

struct Lesson_Full
{
    Lesson lesson;
    int day_of_the_week;
    Lesson_Full() {};
};

struct Time_Archiver
{
    int year;
    int month;
    int mday;
    int wday;
template<class Archive>
void serialize(Archive & ar, const unsigned int version)
{
    ar & mday;
    ar & month;
    ar & year;
    ar & wday;
}
    Time_Archiver() { };
    Time_Archiver(std::tm tm);
    std::tm to_tm();
};

inline bool operator==(const Lesson& lhs, const Lesson& rhs) { return lhs.internal_lesson_id == rhs.internal_lesson_id && lhs.merged_lesson_id == rhs.merged_lesson_id; }
inline bool operator!=(const Lesson& lhs, const Lesson& rhs) { return !(lhs == rhs); }
inline bool operator==(const JTime& lhs, const JTime& rhs) { return lhs.hours==rhs.hours && lhs.minutes==rhs.minutes; }
inline bool operator!=(const JTime& lhs, const JTime& rhs) { return !(lhs == rhs); }
inline bool operator< (const JTime& lhs, const JTime& rhs) { return lhs.hours < rhs.hours || (lhs.hours == rhs.hours && lhs.minutes < rhs.minutes); }
inline bool operator> (const JTime& lhs, const JTime& rhs) { return rhs < lhs; }
inline bool operator<=(const JTime& lhs, const JTime& rhs) { return !(lhs > rhs); }
inline bool operator>=(const JTime& lhs, const JTime& rhs) { return !(lhs < rhs); }

inline bool operator==(const std::tm& lhs, const std::tm& rhs) { return lhs.tm_mday == rhs.tm_mday && lhs.tm_mon == rhs.tm_mon && lhs.tm_year == rhs.tm_year; }
inline bool operator!=(const std::tm& lhs, const std::tm& rhs) { return !(lhs == rhs); }
inline bool operator< (const std::tm& lhs, const std::tm& rhs)
{
    if (lhs.tm_year != rhs.tm_year) return lhs.tm_year < rhs.tm_year;
    if (lhs.tm_mon != rhs.tm_mon) return lhs.tm_mon < rhs.tm_mon;
    return lhs.tm_mday < rhs.tm_mday;
}
inline bool operator> (const std::tm& lhs, const std::tm& rhs) { return rhs < lhs; }
inline bool operator<=(const std::tm& lhs, const std::tm& rhs) { return !(lhs > rhs); }
inline bool operator>=(const std::tm& lhs, const std::tm& rhs) { return !(lhs < rhs); }

const char* c_str_int(int num);
std::string to_string(JTime value);
std::string to_string(JTime begin, JTime end);
std::string to_string(std::tm day, JTime begin, JTime end = {-1, -1}, bool abbreviate = true);
std::string to_string(const std::vector<std::string>& strings, std::string separator = "", const std::vector<bool>& show_only = {});
std::string generate_label(const std::string prefix, std::vector<int> unique);
int validate_time_int(std::string input_string, int upper_limit);
template <typename T = int>
bool is_in_vector(std::vector<T> vector, T to_find);
int get_first_wday(int month, int year, int wday);
int get_number_of_days(int month, int year_starting_from_zero);
int get_first_mwday(int month, int year);
int get_wday(int day, int month, int year);
int get_mday_index_for_wday(int mday, int wday, int month, int year);
int get_wday_count_in_month(int wday, int month, int year);
void next_month_for(int& month, int& year);
void previous_month_for(int& month, int& year);
void HelpMarker(const char *desc);

int loop_plus(int value, int base)
{
    IM_ASSERT(value < base && value >= 0);
    return ((value + 1) % base);
}
int loop_minus(int value, int base)
{
    IM_ASSERT(value < base && value >= 0);
    return ((value + (base - 1)) % base);
}

struct
{
    const time_t timestamp = std::time(NULL);
    const std::tm time = *std::localtime(&timestamp);
} Now;

class Year
{
    int m_value_from_1900;
    Year(int value_from_1900) : m_value_from_1900(value_from_1900)
    {
        IM_ASSERT(value_from_1900 > 0);
    }
public:
    static Year make_from_0(int value) { return Year(value - 1900); }
    static Year make_from_1900(int value) { return Year(value); }
    static Year make_current() { return Year(Now.time.tm_year); }
    int get_from_0() { return m_value_from_1900 + 1900; }
    int get_from_1900() { return m_value_from_1900; }
    void next() { m_value_from_1900++; }
    void previous() { m_value_from_1900--; }
};

class Mday;
class Month
{
public:
    static const int COUNT = 12;
private:
    const std::array<std::string, COUNT> NAMES = 
        {"Январь", 
        "Февраль", 
        "Март", 
        "Апрель", 
        "Май", 
        "Июнь", 
        "Июль", 
        "Август", 
        "Сентябрь", 
        "Октябрь", 
        "Ноябрь", 
        "Декабрь"};
    int m_value_from_0;
    Year m_year;
    Month(int value_from_0, Year year) : m_value_from_0(value_from_0), m_year(year)
    {
        IM_ASSERT(value_from_0 >= 0 && value_from_0 < COUNT);
    }
public:
    static Month make_from_0(int value, Year year = Year::make_current()) { return Month(value, year); }
    static Month make_from_1(int value, Year year = Year::make_current()) { return Month(value - 1, year); }
    static Month make_current() { return Month(Now.time.tm_mon, Year::make_current()); }
    int get_from_0() { return m_value_from_0; }
    int get_from_1() { return m_value_from_0 + 1; }
    std::string name() { return NAMES[m_value_from_0]; }
    Year get_year() { return m_year; }
    void next()
    {
        loop_plus(m_value_from_0, COUNT);
        if (m_value_from_0 == 0) m_year.next();
    }
    void previous()
    {
        if (m_value_from_0 == 0) m_year.previous();
        loop_minus(m_value_from_0, COUNT);
    }
    int get_day_count()
    {
        //int get_number_of_days(int month, int year_starting_from_zero)
        int month = get_from_1();
        int year = m_year.get_from_0();
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
    int calculate_wday_count(Wday wday)
    {
        //int get_wday_count_in_month(int wday, int month, int year)
        return (get_day_count() - Mday::make_from_first_wday(wday, *this).get_from_1()) / Wday::COUNT + 1;
    }
};

class Mday
{
    static const int MAX_COUNT = 31;
    int m_value_from_0;
    Month m_month;
    Mday(int value_from_0, Month month) : m_value_from_0(value_from_0), m_month(month)
    {
        IM_ASSERT(value_from_0 >= 0 && value_from_0 < month.get_day_count());
    }
public:
    static Mday make_from_0(int value, Month month = Month::make_current()) { return Mday(value, month); }
    static Mday make_from_1(int value, Month month = Month::make_current()) { return Mday(value - 1, month); }
    static Mday make_current() { return Mday(Now.time.tm_mday - 1, Month::make_current()); }
    static Mday make_first(Month month = Month::make_current()) { return Mday(0, month); }
    static Mday make_from_first_wday(Wday wday, Month month = Month::make_current())
    {
        // int get_first_wday(int month, int year, int wday)
        std::tm time_in = { 0, 0, 0, // second, minute, hour
            1, month.get_from_0(), month.get_year().get_from_1900() }; // 1-based day, 0-based month, year since 1900

        std::time_t time_temp = std::mktime(&time_in);

        //Note: Return value of localtime is not threadsafe, because it might be
        // (and will be) reused in subsequent calls to std::localtime!
        const std::tm time_out = *std::localtime(&time_temp);
        int first_mday_wday = time_out.tm_wday;
        int diff = ( ( wday.get_EN() - first_mday_wday ) + 7 ) % 7;
        return Mday(diff, month); // TODO: TEST THIS
    }
    int get_from_0() { return m_value_from_0; }
    int get_from_1() { return m_value_from_0 + 1; }
    Month get_month() { return m_month; }
    Year get_year() { return m_month.get_year(); }
    int get_index_in_month() { return get_from_0() / Wday::COUNT; }
    bool next()
    {
        int day_count = get_month().get_day_count();
        if (m_value_from_0 + 1 >= day_count) return false;
        m_value_from_0++;
        return true;
    }
    bool next_week() // same wday
    {
        int day_count = get_month().get_day_count();
        if (m_value_from_0 + 7 >= day_count) return false;
        m_value_from_0 += 7;
        return true;
    }
};

class Wday
{
public:
    static const int COUNT = 7;
private:
    const std::array<std::string, COUNT> NAMES = 
        {"Воскресенье", 
        "Понедельник", 
        "Вторник", 
        "Среда", 
        "Четверг", 
        "Пятница", 
        "Суббота"};
    const std::array<std::string, COUNT> NAMES_SHORT = 
        {"Вс", 
        "Пн", 
        "Вт", 
        "Ср", 
        "Чт", 
        "Пт", 
        "Сб"};
    int m_value_EN;
    Wday(int value_EN) : m_value_EN(value_EN)
    {
        IM_ASSERT(value_EN < COUNT && value_EN >= 0);
    }
public:
    int get_EN() { return m_value_EN; }
    int get_RU() { return loop_minus(m_value_EN, COUNT); }
    static Wday make_from_EN(int value) { return Wday(value); }
    static Wday make_from_RU(int value) { return Wday(loop_plus(value, COUNT)); }
    static Wday make_current() { return Wday(Now.time.tm_wday); }
    static Wday make_from_mday(Mday mday)
    {
    //int get_wday(int day, int month, int year)
        std::tm time_in = { 0, 0, 0, // second, minute, hour
            mday.get_from_1(), mday.get_month().get_from_0(), mday.get_year().get_from_1900() }; // 1-based day, 0-based month, year since 1900
        std::time_t time_temp = std::mktime(&time_in);
        //Note: Return value of localtime is not threadsafe, because it might be
        // (and will be) reused in subsequent calls to std::localtime!
        const std::tm * time_out = std::localtime(&time_temp);
        return time_out->tm_wday;
    }
    static Wday make_from_first_mday(Month month)
    {
        //int get_first_mwday(int month, int year)
        //return get_wday(0, month, year);
        return make_from_mday(Mday::make_from_0(0, month));
    }
    void next() { loop_plus(m_value_EN, COUNT); }
    void previous() { loop_minus(m_value_EN, COUNT); }
    std::string name() { return NAMES[m_value_EN]; }
    std::string name_short() { return NAMES_SHORT[m_value_EN]; }
};
class WdayIterator
{
    bool m_en;
    Wday m_wday;
    bool m_valid = true;
    WdayIterator(bool en, Wday wday) : m_en(en), m_wday(wday) { }
public:
    bool next()
    {
        IM_ASSERT(m_valid);
        m_wday.next();
        if (m_en ? m_wday.get_EN() == 0 : m_wday.get_RU() == 0) m_valid = false;
        return m_valid;
    }
    Wday get()
    { 
        IM_ASSERT(m_valid);
        return m_wday;
    }
    static WdayIterator make_from_RU() { return WdayIterator(false, Wday::make_from_RU(0)); }
    static WdayIterator make_from_EN() { return WdayIterator(true, Wday::make_from_EN(0)); }
};

int get_first_wday(int month, int year, int wday);
int get_number_of_days(int month, int year_starting_from_zero);
int get_first_mwday(int month, int year);
int get_wday(int day, int month, int year);
int get_mday_index_for_wday(int mday, int wday, int month, int year);
int get_wday_count_in_month(int wday, int month, int year);
void next_month_for(int& month, int& year);
void previous_month_for(int& month, int& year);

#include "images.h"
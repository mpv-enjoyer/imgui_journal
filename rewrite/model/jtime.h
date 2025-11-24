#pragma once
#include <cassert>
#include <array>
#include <string>
#include <ctime>
#include "common/assert.h"
#include "common/modifiers.h"

struct
{
    const time_t timestamp = std::time(NULL);
    const std::tm time = *std::localtime(&timestamp);
} Now;

class Year;
class Month;
class Mday;
class Wday;
class Aday;

// Return value is one if Loop was performed
struct Loop
{
    static bool plus(int& value, int base)
    {
        IM_ASSERT(value < base && value >= 0);
        value = ((value + 1) % base);
        return value == 0;
    }
    static bool minus(int& value, int base)
    {
        IM_ASSERT(value < base && value >= 0);
        value = ((value + (base - 1)) % base);
        return value == (base - 1); 
    }
};

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
    int get_from_0() const { return m_value_from_1900 + 1900; }
    int get_from_1900() const { return m_value_from_1900; }
    void next() { m_value_from_1900++; }
    void previous() { m_value_from_1900--; }
    AUTOOPS1(Year, m_value_from_1900);
};

class Month
{
public:
    static constexpr int COUNT = 12;
    static constexpr int BEGIN_STUDY_MONTH_FROM_0 = 8;
    static constexpr int END_STUDY_MONTH_FROM_0 = BEGIN_STUDY_MONTH_FROM_0 - 1;
private:
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
    static Month make_begin_study_year(Month month) { return make_begin_study_year_from_bottom_year(month.get_study_bottom_year()); }
    static Month make_begin_study_year_from_bottom_year(Year bottom_year) { return Month(BEGIN_STUDY_MONTH_FROM_0, bottom_year); }
    int get_from_0() const { return m_value_from_0; }
    int get_from_1() const { return m_value_from_0 + 1; }
    Year get_study_bottom_year()
    {
        Year copy = m_year;
        if (m_value_from_0 < BEGIN_STUDY_MONTH_FROM_0) copy.previous();
        return copy;
    }
    std::size_t calculate_study_year_index() const
    {
        return (m_value_from_0 + COUNT - BEGIN_STUDY_MONTH_FROM_0) % COUNT;
    }
    std::string get_name() const
    {
        switch (m_value_from_0)
        {
            case 0: return "Январь";
            case 1: return "Февраль";
            case 2: return "Март";
            case 3: return "Апрель";
            case 4: return "Май";
            case 5: return "Июнь";
            case 6: return "Июль";
            case 7: return "Август";
            case 8: return "Сентябрь";
            case 9: return "Октябрь";
            case 10: return "Ноябрь";
            case 11: return "Декабрь";
            default: IM_ASSERT(false);
        }
    }
    Year get_year() const { return m_year; }
    bool next()
    {
        if (Loop::plus(m_value_from_0, COUNT)) m_year.next();
        return m_value_from_0 != BEGIN_STUDY_MONTH_FROM_0;
    }
    bool previous()
    {
        if (Loop::minus(m_value_from_0, COUNT)) m_year.previous();
        return m_value_from_0 != END_STUDY_MONTH_FROM_0;
    }
    bool next_bounded()
    {
        bool result = next();
        if (!result) previous();
        return result;
    }
    bool previous_bounded()
    {
        bool result = previous();
        if (!result) next();
        return result;
    }
    int get_day_count() const
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
    int calculate_wday_count(Wday wday) const;
    AUTOOPS2(Month, m_year, m_value_from_0);
};

// Attendance day. Represents one mday in an array of
// days with the same wday starting at the study year.
class Aday // TODO: STILL UNTESTED
{
    std::size_t m_index = 0;
    Aday(size_t index) : m_index(index) { }
public:
    static Aday make_from_index(size_t index) { return Aday(index); }
    static Aday make_from_mday(Mday mday);
    static Aday make_from_first_wday(Wday wday, Month month);
    std::size_t index() const { return m_index; }
    bool next()
    {
        m_index++;
        return true;
    }
    AUTOEQ1(Aday, m_index);
};

class Mday
{
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
    static Mday make_from_first_wday(Wday wday, Month month);
    static Mday make_from_aday(Year bottom_year, Wday wday, Aday aday);
    int get_from_0() const { return m_value_from_0; }
    int get_from_1() const { return m_value_from_0 + 1; }
    Month get_month() const { return m_month; }
    Year get_year() const { return m_month.get_year(); }
    int get_index_in_month() const;
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
    AUTOOPS2(Mday, m_month, m_value_from_0);
};

class Wday
{
public:
    static constexpr int COUNT = 7;
private:
    int m_value_EN;
    bool m_ru; // Is iterating from Russian offset? CONST.
    Wday(int value_EN, bool ru = false) : m_value_EN(value_EN), m_ru(ru)
    {
        IM_ASSERT(value_EN < COUNT && value_EN >= 0);
    }
public:
    int get_EN() const { return m_value_EN; }
    int get_RU() const { int temp_value_EN = m_value_EN; Loop::minus(temp_value_EN, COUNT); return temp_value_EN; }
    static Wday make_from_EN(int value) { return Wday(value); }
    static Wday make_from_RU(int value) { Loop::plus(value, COUNT); return Wday(value); }
    static Wday make_begin_EN() { return make_from_EN(0); }
    static Wday make_begin_RU() { return make_from_RU(0); }
    static Wday make_current() { return Wday(Now.time.tm_wday); }
    static Wday make_from_mday(Mday mday)
    {
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
        return make_from_mday(Mday::make_from_0(0, month));
    }
    bool next()
    {
        Loop::plus(m_value_EN, COUNT);
        return m_ru ? get_RU() != 0 : get_EN() != 0;
    }
    bool previous()
    {
        bool value_return = m_ru ? get_RU() != 0 : get_EN() != 0;
        Loop::minus(m_value_EN, COUNT);
        return value_return;
    }
    std::string get_name() const
    {
        switch (m_value_EN)
        {
        case 0: return "Воскресенье";
        case 1: return "Понедельник";
        case 2: return "Вторник";
        case 3: return "Среда";
        case 4: return "Четверг";
        case 5: return "Пятница";
        case 6: return "Суббота";
        default: IM_ASSERT(false);
        }
    }
    std::string get_name_short() const
    {
        switch (m_value_EN)
        {
        case 0: return "Вс";
        case 1: return "Пн";
        case 2: return "Вт";
        case 3: return "Ср";
        case 4: return "Чт";
        case 5: return "Пт";
        case 6: return "Сб";
        default: IM_ASSERT(false);
        }
    }
    std::size_t calculate_count_for_bottom_year(Year bottom_year) const
    {
        std::size_t count = 0;
        auto month = Month::make_begin_study_year_from_bottom_year(bottom_year);
        do
        {
            count += month.calculate_wday_count(*this);
        } while (month.next());
        return count;
    }
    AUTOOPS1(Wday, m_value_EN);
};

class JTime
{
    int m_hours; //0-23
    int m_minutes; //0-59
    int bounded(int value, int max)
    {
        if (value >= max) return max - 1;
        if (value < 0) return 0;
        return value;
    }
public:
    JTime(int hours, int minutes)
    : m_hours(bounded(hours, 24)),
      m_minutes(bounded(minutes, 60))
    { }
    int get_minutes() const { return m_minutes; }
    int get_hours() const { return m_hours; }
    AUTOOPS2(JTime, m_hours, m_minutes);
};

inline int Month::calculate_wday_count(Wday wday) const
{
    //int get_wday_count_in_month(int wday, int month, int year)
    return (get_day_count() - Mday::make_from_first_wday(wday, *this).get_from_1()) / Wday::COUNT + 1;
}

inline Aday Aday::make_from_mday(Mday mday)
{
    auto wday = Wday::make_from_mday(mday);
    size_t index = 0;
    for (auto month = Month::make_begin_study_year(mday.get_month()); month != mday.get_month(); month.next())
    {
        index += month.calculate_wday_count(wday);
    }
    index += mday.get_index_in_month();
    return Aday(index);
}

inline Aday Aday::make_from_first_wday(Wday wday, Month month)
{
    Mday mday = Mday::make_from_first_wday(wday, month);
    return make_from_mday(mday);
}

inline Mday Mday::make_from_first_wday(Wday wday, Month month)
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

inline Mday Mday::make_from_aday(Year bottom_year, Wday wday, Aday aday)
{
    std::size_t aday_index = aday.index();
    std::size_t aday_index_current = 0;
        //auto wday = Wday::make_from_mday();
    Month month = Month::make_begin_study_year_from_bottom_year(bottom_year);
    for (; aday_index_current + month.calculate_wday_count(wday) < aday_index; month.next())
    {
        aday_index_current += month.calculate_wday_count(wday);
    }
    std::size_t first_mday_from_0 = Mday::make_from_first_wday(wday, month).get_from_0();
    return Mday::make_from_0(first_mday_from_0 + (aday_index_current - aday_index) * Wday::COUNT, month); // TODO: TEST THIS
}

inline int Mday::get_index_in_month() const
{
    return get_from_0() / Wday::COUNT;
}


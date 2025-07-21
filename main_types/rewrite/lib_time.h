#include <cassert>
#define IM_ASSERT assert
#include <array>
#include <string>
#include <ctime>

struct
{
    const time_t timestamp = std::time(NULL);
    const std::tm time = *std::localtime(&timestamp);
} Now;

class Year;
class Month;
class Mday;
class Wday;

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
};

class Month
{
public:
    static constexpr int COUNT = 12;
    static constexpr int BEGIN_STUDY_MONTH_FROM_0 = 8;
    static constexpr int END_STUDY_MONTH_FROM_0 = BEGIN_STUDY_MONTH_FROM_0 - 1;
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
    static Month make_begin_study_year(Year bottom_year) { return Month(BEGIN_STUDY_MONTH_FROM_0, bottom_year); }
    int get_from_0() const { return m_value_from_0; }
    int get_from_1() const { return m_value_from_0 + 1; }
    std::string name() const { return NAMES[m_value_from_0]; }
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
};

class Mday
{
    static constexpr int MAX_COUNT = 31;
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
    static Mday make_from_first_wday(Wday wday, Month month = Month::make_current());
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
};

class Wday
{
public:
    static constexpr int COUNT = 7;
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
    const bool m_ru;
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
    std::string name() { return NAMES[m_value_EN]; }
    std::string name_short() { return NAMES_SHORT[m_value_EN]; }
};
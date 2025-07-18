#include "lib_time.h"
#include <iostream>

int main()
{
    std::cout << "Today is ";
    std::cout << Mday::make_current().get_from_1();
    std::cout << " ";
    std::cout << Month::make_current().name();
    std::cout << " ";
    std::cout << Year::make_current().get_from_0();
    std::cout << "y\n";

    std::cout << "The remaining days are: ";
    Mday mday = Mday::make_current();
    do
    {
        std::cout << mday.get_from_1() << " ";
    }
    while (mday.next());
    std::cout << "\n";

    std::cout << "Current wday days are: ";
    Mday mday2 = Mday::make_from_first_wday(Wday::make_current());
    do
    {
        std::cout << mday2.get_from_1() << " ";
    }
    while (mday2.next_week());
    std::cout << "\n";

    std::cout << "Yesterday wday days are: ";
    Wday yesterday = Wday::make_current();
    yesterday.previous();
    Mday mday3 = Mday::make_from_first_wday(yesterday);
    do
    {
        std::cout << mday3.get_from_1() << " ";
    }
    while (mday3.next_week());
    std::cout << "\n";

    auto iter = Wday::make_begin_RU();
    do
    {
        std::cout << "Wday " << iter.name() << " has " << Month::make_current().calculate_wday_count(iter) << " mdays\n";
    }
    while (iter.next());
    std::cout << mday3.get_month().calculate_wday_count(Wday::make_current()) << "\n";

    auto month = Month::make_begin_study_year(Year::make_from_0(2024));
    std::cout << "All mdays for current wday:\n";
    do
    {
        std::cout << " " << month.name() << " ";
        auto mday = Mday::make_from_first_wday(Wday::make_current(), month);
        do
        {
            std::cout << mday.get_from_1() << " ";
        }
        while (mday.next_week());
        std::cout << "\n";
    }
    while (month.next());
}
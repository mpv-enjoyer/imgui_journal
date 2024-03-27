#include "public.h"
#include "internal.h"

namespace Journal
{
    std::string generate_file_name(int month, int year)
    {
        return "save_" + std::to_string(month + 1) + "m_" + std::to_string(year + 1900) + "y.data";
    }

    bool save(int month, int year)
    {
        if (month == -1 || year == -1)
        {
            year = _current_year;
            month = _current_month;
        }
        return false;
    }

    bool load_current()
    {

    }

    void generate_current()
    {

    }
}
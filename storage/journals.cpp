#include "journals.h"
#include <unordered_map>
#include <filesystem>

struct Journal_Year
{
    std::array<Journal*, 12> m_journals_per_month = {};
    Year m_bottom_year;
    Workout_Handler m_workouts;
    Journal_Year(Year bottom_year);
    Year get_bottom_year();
    Month get_month(size_t index);
    std::string get_save_file_name();
};

Journal_Year* m_journal_year = nullptr; // CURRENT YEAR.

Journal_Year::Journal_Year(Year bottom_year) : m_bottom_year(bottom_year)
{
    // TODO: migration
    bool existed = std::filesystem::exists(get_save_file_name());
    bool generate = !existed;

    if (existed)
    {
        for (int i = 0; i < m_journals_per_month.size(); i++) {
            m_journals_per_month[i] = new Journal(get_month(i), generate);
        }
        std::ifstream ifs(get_save_file_name());
        boost::archive::text_iarchive ia(ifs);
        for (int i = 0; i < m_journals_per_month.size(); i++) {
            ia >> m_journals_per_month[i];
        }
        ia >> m_workouts;
    }
    else
    {
        for (int i = 0; i < m_journals_per_month.size(); i++) {
            m_journals_per_month[i] = new Journal(get_month(i), generate);
            m_journals_per_month[i]->generate();
        }
    }
}

Year Journal_Year::get_bottom_year()
{
    return m_bottom_year;
}

Journal *Journal_Year::get_journal(size_t index)
{
    return m_journals_per_month[index];
}

std::string Journal_Year::get_save_file_name()
{
    return "save2_" + std::to_string(m_bottom_year.get_from_0()) + "y.data";
}

Journal *Journals::get_journal(Month month)
{
    Year bottom_year = month.get_study_bottom_year();
    size_t index = month.calculate_study_year_index();
    if (bottom_year == m_current_year.get_bottom_year()) {
        return m_current_year.get_journal(index);
    }
    if (!m_journal_year) {
        
    }
}

bool Journals::is_next_month_in_the_same_study_year(Month month)
{
    return month.get_from_0() != Month::END_STUDY_MONTH_FROM_0;
}

bool Journals::is_next_month_in_the_same_study_year(Journal *journal)
{
    return is_next_month_in_the_same_study_year(get_month(journal));
}

bool Journals::is_previous_month_in_the_same_study_year(Month month)
{
    return month.get_from_0() != Month::BEGIN_STUDY_MONTH_FROM_0;
}

bool Journals::is_previous_month_in_the_same_study_year(Journal *journal)
{
    return is_previous_month_in_the_same_study_year(get_month(journal));
}

Month Journals::get_month(Journal *journal)
{
    return Month::make_from_0(journal->current_month(), Year::make_from_1900(journal->current_year()));
}

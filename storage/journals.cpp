#include "journals.h"
#include <unordered_map>
#include <filesystem>

struct Journal_Year
{
    std::array<Journal*, 12> m_journals_per_month = {};
    Workout_Handler* m_workouts = nullptr;
    Year m_bottom_year;
    Journal_Year(Year bottom_year);
    Year get_bottom_year();
    Month get_month(size_t index);
    std::string get_save_file_name();
    std::string get_legacy_workout_file_name();
    void save();
    ~Journal_Year();
};

std::string get_legacy_prices_file_name(Month month)
{
    return "save_" + std::to_string(month.get_from_1()) + "m_" + std::to_string(month.get_year().get_from_0()) + "y_prices.data";
}

Journal_Year::Journal_Year(Year bottom_year) : m_bottom_year(bottom_year)
{
    bool existed = std::filesystem::exists(get_save_file_name());
    bool existed_legacy = std::filesystem::exists(get_legacy_workout_file_name());
    bool need_migration = !existed && existed_legacy;
    
    if (need_migration) {
        {
            std::ofstream ofs(get_save_file_name());
            boost::archive::text_oarchive oa(ofs);
            for (int i = 0; i < m_journals_per_month.size(); i++) {
                Month month = get_month(i);
                // Load from legacy constructors:
                Journal* journal = (month == Month::make_current()) ? new Journal() : new Journal(month.get_from_0(), month.get_year().get_from_1900(), nullptr);
                journal->restrict_saving = true;
                oa << journal; // it loads Prices too. Fully valid save. Not fully valid object though.
                delete journal;
            }

            std::ifstream ifs_workout(get_legacy_workout_file_name());
            boost::archive::text_iarchive ia(ifs_workout);
            Workout_Handler* dummy_wh = new Workout_Handler();
            ia >> dummy_wh;
            oa << dummy_wh;
            delete dummy_wh;
            existed = true;
        }
        printf("Migration step 1: made valid save file!!\n");
    }

    Month first_month_in_this_bottom_year = Month::make_begin_study_year_from_bottom_year(m_bottom_year);
    m_workouts = new Workout_Handler(first_month_in_this_bottom_year.get_from_0(), m_bottom_year.get_from_1900());
    const Workout_Handler* workouts_debug_ptr = m_workouts;

    if (existed)
    {
        for (int i = 0; i < m_journals_per_month.size(); i++) {
            m_journals_per_month[i] = new Journal(get_month(i), false, m_workouts);
        }
        std::ifstream ifs(get_save_file_name());
        boost::archive::text_iarchive ia(ifs);
        for (int i = 0; i < m_journals_per_month.size(); i++) {
            ia >> m_journals_per_month[i];
        }
        ia >> m_workouts;
        IM_ASSERT(workouts_debug_ptr == m_workouts); // Please tell me that boost doesn't change pointers.

        if (need_migration) {
            // TODO: decide what to do with newly added groups not in september.
            // 1. Do nothing. Not an option as it will screw up newly students or workouts
            // 2. is_deleted() and is_deleted_invisible(), basically store if deleted *thing* must be ever shown.
            //    We need to fill in dummy invisible items all over the save data here.   
        }
    }
    else
    {
        for (int i = 0; i < m_journals_per_month.size(); i++) {
            m_journals_per_month[i] = new Journal(get_month(i), true, m_workouts);
        }
        /* m_workouts is already initialized here */
    }
}

Year Journal_Year::get_bottom_year()
{
    return m_bottom_year;
}

Month Journal_Year::get_month(size_t index)
{
    size_t month_index_from_0 = (Month::BEGIN_STUDY_MONTH_FROM_0 + index) % Month::COUNT;
    Year year = m_bottom_year;
    if (month_index_from_0 <= Month::END_STUDY_MONTH_FROM_0) {
        year.next();
    }
    return Month::make_from_0(month_index_from_0, year);
}

std::string Journal_Year::get_save_file_name()
{
    return "save2_" + std::to_string(m_bottom_year.get_from_0()) + "y.data";
}

std::string Journal_Year::get_legacy_workout_file_name()
{    
    return "save_" + std::to_string(m_bottom_year.get_from_0()) + "y_workouts.data";
}

void Journal_Year::save()
{
    std::ofstream ofs(get_save_file_name());
    boost::archive::text_oarchive oa(ofs);
    for (int i = 0; i < m_journals_per_month.size(); i++) {
        oa << m_journals_per_month[i];
    }
    oa << m_workouts;
}

Journal_Year::~Journal_Year()
{
    save();
    for (Journal* journal : m_journals_per_month) {
        delete journal;
    }
    delete m_workouts;
}


// The one and only, journal storage:
std::unique_ptr<Journal_Year> m_journal_year = std::make_unique<Journal_Year>(Year::make_current_bottom_year());

Journal *Journals::get_journal_cross_year(Month month)
{
    Year bottom_year = month.get_study_bottom_year();
    size_t index = month.calculate_study_year_index();
    if (bottom_year != m_journal_year->get_bottom_year()) {
        m_journal_year = std::make_unique<Journal_Year>(Year::make_current_bottom_year()); // Saving occurs here
    }
    return m_journal_year->m_journals_per_month[index];
}

std::optional<Journal *> Journals::get_next_journal_this_year(Journal *journal)
{
    Month month = get_month(journal);
    bool success = month.next_bounded();
    if (success) {
        size_t index = month.calculate_study_year_index();
        return m_journal_year->m_journals_per_month[index];
    } else {
        return std::nullopt;
    }
}

Journal *Journals::get_next_journal_cross_year(Journal *journal)
{
    Month month = get_month(journal);
    month.next();
    return get_journal_cross_year(month);
}

std::optional<Journal *> Journals::get_previous_journal_this_year(Journal *journal)
{
    Month month = get_month(journal);
    bool success = month.previous_bounded();
    if (success) {
        size_t index = month.calculate_study_year_index();
        return m_journal_year->m_journals_per_month[index];
    } else {
        return std::nullopt;
    }
}

Journal *Journals::get_previous_journal_cross_year(Journal *journal)
{
    Month month = get_month(journal);
    month.previous();
    return get_journal_cross_year(month);
}

Month Journals::get_month(Journal *journal)
{
    return Month::make_from_0(journal->current_month(), Year::make_from_1900(journal->current_year()));
}

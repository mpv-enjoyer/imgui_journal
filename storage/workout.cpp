#include "workout.h"

Workout_Handler::Workout_Handler(int month, int year)
{
    _bottom_year = get_bottom_year(month, year);
    _top_year = _bottom_year + 1;
}

Workout_Handler::Workout_Handler()
{
    
}

int Workout_Handler::get_bottom_year(int month, int year)
{
    int bottom_year = year;
    if (month < STUDY_YEAR_BEGIN_MONTH) bottom_year--;
    return bottom_year;
}

int Workout_Handler::bottom_year()
{
    return _bottom_year;
}

int Workout_Handler::top_year()
{
    return _top_year;
}

bool Workout_Handler::is_month_here(int month, int year)
{
    int want_bottom_year = year;
    if (month < STUDY_YEAR_BEGIN_MONTH) want_bottom_year--;
    return want_bottom_year == _bottom_year;
}

int Workout_Handler::get_year(int month) const
{
    return month >= STUDY_YEAR_BEGIN_MONTH ? _bottom_year : _top_year;
}

void Workout_Handler::insert_info(Workout_Info_ workout_info)
{
    IM_ASSERT(workout_info.real_attend.tm_wday < 7);
    IM_ASSERT(workout_info.should_attend.tm_wday < 7);
    _all_workouts.push_back(workout_info);
}

std::vector<std::vector<std::pair<const Workout_Info_*, const Workout_Info_*>>> Workout_Handler::get_info(int real_month, int real_wday, int real_merged_lesson, std::vector<int>& student_ids)
{
    Lesson lesson = {.merged_lesson_id = real_merged_lesson, .internal_lesson_id = 0};
    std::vector<int> temp_student_ids1;
    std::vector<std::vector<const Workout_Info_*>> info1 = get_info(real_month, real_wday, lesson, &temp_student_ids1);
    student_ids = temp_student_ids1;
    lesson.internal_lesson_id = 1;
    std::vector<int> temp_student_ids2;
    std::vector<std::vector<const Workout_Info_*>> info2 = get_info(real_month, real_wday, lesson, &temp_student_ids2);
    for (auto id : temp_student_ids2)
    {
        if (std::find(student_ids.begin(), student_ids.end(), id) == student_ids.end()) student_ids.push_back(id);
    }
    // All ids are in list, now fill the list.
    // Sorting students by id to avoid weird replacement after removing / inserting workouts
    std::sort(student_ids.begin(), student_ids.end());

    const int output_width = get_wday_count_in_month(real_wday, real_month, get_year(real_month));
    // We have ids list for every info stored in temp vectors, use them to navigate
    std::vector<std::vector<std::pair<const Workout_Info_*, const Workout_Info_*>>> output =
    std::vector<std::vector<std::pair<const Workout_Info_*, const Workout_Info_*>>>
    (student_ids.size(), std::vector<std::pair<const Workout_Info_*, const Workout_Info_*>>
    (output_width, std::pair<const Workout_Info_*, const Workout_Info_*>
    ({nullptr, nullptr})));
    for (int i = 0; i < info1.size(); i++)
    {
        int student_id = temp_student_ids1[i];
        int output_index = std::distance(student_ids.begin(), std::find(student_ids.begin(), student_ids.end(), student_id));
        for (int j = 0; j < info1[i].size(); j++)
        {
            auto current = info1[i][j];
            if (current == nullptr) continue;
            output[output_index][j].first = current;
        }
    }
    for (int i = 0; i < info2.size(); i++)
    {
        int student_id = temp_student_ids2[i];
        int output_index = std::distance(student_ids.begin(), std::find(student_ids.begin(), student_ids.end(), student_id));
        for (int j = 0; j < info2[i].size(); j++)
        {
            auto current = info2[i][j];
            if (current == nullptr) continue;
            output[output_index][j].second = current;
        }
    }
    return output;
}

std::vector<std::vector<const Workout_Info_*>> Workout_Handler::get_info(int real_month, int real_wday, Lesson real_lesson, std::vector<int>* student_ids)
{
    std::vector<int> _student_ids;
    std::vector<std::vector<const Workout_Info_*>> output;
    int current_year = get_year(real_month);
    const int output_width = get_wday_count_in_month(real_wday, real_month, current_year);
    for (auto& workout : _all_workouts)
    {
        if (workout.real_attend.tm_mon != real_month) continue;
        if (workout.real_attend.tm_wday != real_wday) continue;
        if (workout.real_lesson != real_lesson) continue;
        int workout_student_id = workout.real_student_id;
        int found_student_id = -1;
        for (int i = 0; i < _student_ids.size(); i++)
        {
            if (_student_ids[i] == workout_student_id)
            {
                found_student_id = i;
                break;
            }
        }
        int index = get_mday_index_for_wday(workout.real_attend.tm_mday, real_wday, real_month, current_year);
        if (found_student_id == -1)
        {
            output.push_back(std::vector<const Workout_Info_*>(output_width, nullptr));
            output[output.size() - 1][index] = &workout;
            _student_ids.push_back(workout_student_id);
        }
        else
        {
            output[found_student_id][index] = &workout;
        }
    }
    if (student_ids != nullptr) *student_ids = _student_ids;
    return output;
}

const Workout_Info_* Workout_Handler::get_info(int should_month, int should_mday, Lesson should_lesson, int should_student_id)
{
    for (const auto& workout : _all_workouts)
    {
        if (workout.should_attend.tm_mon != should_month) continue;
        if (workout.should_attend.tm_mday != should_mday) continue;
        if (workout.should_lesson != should_lesson) continue;
        if (workout.should_student_id != should_student_id) continue;
        return &workout;
    };
    return nullptr;
}

const std::vector<const Workout_Info_ *> Workout_Handler::search_info(int should_month)
{
    std::vector<const Workout_Info_*> output;
    for (const auto& info : _all_workouts)
    {
        if (info.should_attend.tm_mon == should_month) output.push_back(&(info));
    }
    return output;
}

void Workout_Handler::delete_info(const Workout_Info_ *workout_info)
{
    int index = -1;
    for (int i = 0; i < _all_workouts.size(); i++)
    {
        if (&(_all_workouts[i]) == workout_info) index = i; // just in case
    }
    IM_ASSERT(index != -1 && "workout_info* doesn't exist. It was probably deallocated by vector.");
    auto iter = _all_workouts.begin() + index;
    if (iter != _all_workouts.end()) _all_workouts.erase(iter);
}

bool Workout_Handler::remove_student(int month, int student_id)
{
    // check if student is present in workouts (it shouldn't)
    for (auto& workout : _all_workouts)
    {
        if (workout.should_attend.tm_mon != month) continue;
        if (workout.should_student_id == student_id || workout.real_student_id == student_id) return false;
    }

    std::vector<std::pair<Workout_Info_, const Workout_Info_*>> new_and_to_remove;
    for (auto& workout : _all_workouts)
    {
        if (workout.should_attend.tm_mon != month) continue;
        if (workout.should_student_id < student_id && workout.real_student_id < student_id) continue;
        Workout_Info_ new_info = workout;
        if (new_info.real_student_id > student_id) new_info.real_student_id--;
        if (new_info.should_student_id > student_id) new_info.should_student_id--;
        new_and_to_remove.push_back({new_info, &workout});
    }
    
    for (auto info : new_and_to_remove)
    {
        delete_info(info.second);
    }

    for (auto info : new_and_to_remove)
    {
        insert_info(info.first);
    }
    return true;
}

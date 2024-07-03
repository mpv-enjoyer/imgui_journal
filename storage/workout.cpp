#include "workout.h"

void Workout_Handler::insert_info(Workout_Info_ workout_info)
{
    _all_workouts.push_back(workout_info);
    Workout_Info_* last_element = &(_all_workouts[_all_workouts.size() - 1]);
    Workout_Hash_Container container = { last_element };
    _real_hashes.insert(container);
    _last_real_hashes.insert(container);
    _should_hashes.insert(container);
}

int get_mday_index_for_wday(int mday, int wday, int month, int year)
{
    int day = get_first_wday(month, year, wday) - MDAY_DIFF;
    int day_count = get_number_of_days(month, year);
    for (int i = 0; day <= day_count; day += 7, i++)
    {
        if (day == mday) return i;
    }
    return -1;
}

std::vector<std::vector<const Workout_Info_*>> Workout_Handler::get_info(int real_month, int real_wday, Lesson real_lesson)
{
    Workout_Info_ request;
    request.real_lesson = real_lesson;
    request.real_attend.tm_wday = real_wday;
    request.real_attend.tm_mon = real_month;
    Workout_Hash_Container container = { &request };
    auto real_result = _real_hashes.find(container);
    std::vector<std::vector<const Workout_Info_*>> output;
    for (; real_result != _real_hashes.end(); ++real_result)
    {
        request.student_id = real_result->info->student_id;
        auto last_result = _last_real_hashes.find(container);
        int current_year = real_month >= STUDY_YEAR_BEGIN_MONTH ? _bottom_year : _top_year;
        int day_count = get_number_of_days(real_month, current_year);
        if (last_result == _last_real_hashes.end()) continue;
        output.push_back(std::vector<const Workout_Info_*>(day_count, nullptr));
        for (; last_result != _last_real_hashes.end(); ++last_result)
        {
            int index = get_mday_index_for_wday(last_result->info->real_attend.tm_mday, real_wday, real_month, current_year);
            IM_ASSERT(output[output.size() - 1][index] == nullptr);
            output[output.size() - 1][index] = last_result->info; 
        }
    }
    return output;
}

void Workout_Handler::delete_info(const Workout_Info_ *workout_info)
{
    //raw data access here!
    Workout_Hash_Container container = { workout_info };
    _real_hashes.erase(container); //TODO IMPORTANT: check if equal hashes are removed
    _last_real_hashes.erase(container);
    _should_hashes.erase(container);
    int index = workout_info - _all_workouts.data();
    auto iter = _all_workouts.begin() + index;
    _all_workouts.erase(iter);
}

#include "workout.h"

void Workout_Handler::insert_info(Workout_Info_ workout_info)
{
    IM_ASSERT(workout_info.real_attend.tm_wday < 7);
    IM_ASSERT(workout_info.should_attend.tm_wday < 7);
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

const Workout_Info_* Workout_Handler::get_info(int should_month, std::tm should_day, Lesson should_lesson, const Student* student)
{
    Workout_Info_ request;
    request.should_attend.tm_mon = should_month;
    request.should_attend.tm_mday = should_day.tm_mday;
    request.should_lesson = should_lesson;
    Workout_Hash_Container container = { &request };
    auto found = _should_hashes.find(container);
    if (found == _should_hashes.end()) return nullptr;
    return found->info;
}

void Workout_Handler::delete_info(const Workout_Info_ *workout_info)
{
    Workout_Hash_Container container = { workout_info };
    _real_hashes.erase(container); //TODO IMPORTANT: check if equal hashes are removed
    _last_real_hashes.erase(container);
    _should_hashes.erase(container);
    int index = workout_info - _all_workouts.data();
    auto iter = _all_workouts.begin() + index;
    _all_workouts.erase(iter);
}

void Workout_Handler::change_lesson_info_position(int month, int wday, int old_merged_lesson_id,
    int new_merged_lesson_id, bool is_new, int max_merged_lessons_size)
{
    if (old_merged_lesson_id == new_merged_lesson_id) return;
    bool moved_right = new_merged_lesson_id > old_merged_lesson_id;
    int min_affected = moved_right ? old_merged_lesson_id : new_merged_lesson_id;
    int max_affected = moved_right ? new_merged_lesson_id : old_merged_lesson_id;
    if (is_new) // newly created lesson
    {
        max_affected
        min_affected = new_merged_lesson_id;
    }
    std::vector<std::pair<Workout_Info_, const Workout_Info_*>> new_and_to_remove;
    for (int merged_lesson = min_affected; merged_lesson <= max_affected; merged_lesson++)
    {
        bool decrement = moved_right && (merged_lesson != min_affected);
        bool increment = !moved_right && (merged_lesson != max_affected);
        bool to_max = moved_right && merged_lesson == min_affected;
        bool to_min = !moved_right && merged_lesson == max_affected;
        for (int internal_lesson = 0; internal_lesson < MAX_INTERNAL_LESSONS; internal_lesson++)
        {
            Lesson lesson = {.merged_lesson_id = merged_lesson, .internal_lesson_id = internal_lesson};
            auto info = get_info(month, wday, lesson);
            for (int i = 0; i < info.size(); i++)
            {
                for (int j = 0; j < info[i].size(); j++)
                {
                    Workout_Info_ new_info = *(info[i][j]);

                    if (decrement) new_info.real_lesson.merged_lesson_id--;
                    else if (increment) new_info.real_lesson.merged_lesson_id++;
                    else if (to_max) new_info.real_lesson.merged_lesson_id = max_affected;
                    else if (to_min) new_info.real_lesson.merged_lesson_id = min_affected;
                    else throw std::invalid_argument("");

                    new_and_to_remove.push_back({new_info, info[i][j]});
                }
            }
        }
    }
    for (auto info : new_and_to_remove)
    {
        delete_info(info.second);
        insert_info(info.first);
    }
    std::vector<std::pair<Workout_Info_, const Workout_Info_*>> new_and_to_remove;
    for (const auto& workout : _all_workouts)
    {
        if (workout.should_attend.tm_wday != wday) continue;
        if (workout.should_attend.tm_mon != month) continue;
        if (workout.should_lesson.merged_lesson_id < min_affected) continue;
        if (workout.should_lesson.merged_lesson_id > max_affected) continue;
        int merged_lesson = workout.should_lesson.merged_lesson_id;
        bool decrement = moved_right && (merged_lesson != min_affected);
        bool increment = !moved_right && (merged_lesson != max_affected);
        bool to_max = moved_right && merged_lesson == min_affected;
        bool to_min = !moved_right && merged_lesson == max_affected;

        Workout_Info_ new_info = workout;

        if (decrement) new_info.real_lesson.merged_lesson_id--;
        else if (increment) new_info.real_lesson.merged_lesson_id++;
        else if (to_max) new_info.real_lesson.merged_lesson_id = max_affected;
        else if (to_min) new_info.real_lesson.merged_lesson_id = min_affected;
        else throw std::invalid_argument("");

        new_and_to_remove.push_back({new_info, &workout});
    }
    for (auto workout : new_and_to_remove)
    {
        delete_info(workout.second);
        insert_info(workout.first);
    }
}


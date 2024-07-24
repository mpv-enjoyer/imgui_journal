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
    Workout_Hash_Container container = { &_all_workouts, _all_workouts.size() - 1 };
    _real_hashes.insert(container);
    _last_real_hashes.insert(container);
    _should_hashes.insert(container);
}

std::vector<std::vector<const Workout_Info_*>> Workout_Handler::get_info(int real_month, int real_wday, Lesson real_lesson)
{
    std::vector<Workout_Info_> request(1);
    request[0].real_lesson = real_lesson;
    request[0].real_attend.tm_wday = real_wday;
    request[0].real_attend.tm_mon = real_month;
    Workout_Hash_Container container = { &request, 0 };
    auto real_hash = _real_hashes.bucket(container);
    auto real_result = _real_hashes.begin(real_hash);
    std::vector<std::vector<const Workout_Info_*>> output;
    for (auto iter = real_result; iter != _real_hashes.end(real_hash); ++iter)
    {
        request[0].real_student_id = iter->info().real_student_id;
        auto last_hash = _last_real_hashes.bucket(container);
        auto last_result = _last_real_hashes.begin(last_hash);
        int current_year = get_year(real_month);
        if (last_result == _last_real_hashes.end(last_hash)) continue;

        std::vector<const Workout_Info_*> possible_append(get_wday_count_in_month(real_wday, real_month, current_year), nullptr);
        bool found_equal_workouts = false;
        
        for (auto last_iter = last_result; last_iter != _last_real_hashes.end(last_hash); ++last_iter)
        {
            // Here we check if values are actually valid (searched by hash equality but need the exact results).
            if (last_iter->info().real_attend.tm_wday != real_wday) continue;
            if (last_iter->info().real_student_id != request[0].real_student_id) continue;
            if (last_iter->info().real_attend.tm_mon != real_month) continue;
            if (last_iter->info().real_lesson != real_lesson) continue;
            found_equal_workouts = true;
            int index = get_mday_index_for_wday(last_iter->info().real_attend.tm_mday, real_wday, real_month, current_year);
            IM_ASSERT(possible_append[index] == nullptr);
            possible_append[index] = &(last_iter->all_workouts->at(last_iter->index)); //WARNING: this pointer will break after vector reallocation
            IM_ASSERT(possible_append[index]->real_lesson == real_lesson);
        }
        if (found_equal_workouts) output.push_back(possible_append);
    }
    return output;
}

const Workout_Info_* Workout_Handler::get_info(int should_month, int should_mday, Lesson should_lesson, int should_student_id)
{
    std::vector<Workout_Info_> request(1);
    request[0].should_attend.tm_mon = should_month;
    request[0].should_attend.tm_mday = should_mday;
    request[0].should_lesson = should_lesson;
    request[0].should_student_id = should_student_id;
    Workout_Hash_Container container = { &request, 0 };
    auto hash = _should_hashes.bucket(container);
    auto iter = _should_hashes.begin(hash);
    for (; iter != _should_hashes.end(hash); ++iter)
    {
        // Here we check if values are actually valid (searched by hash equality but need the exact results).
        if (iter->info().should_attend.tm_mon != should_month) continue;
        if (iter->info().should_attend.tm_mday != should_mday) continue;
        if (iter->info().should_lesson != should_lesson) continue;
        if (iter->info().should_student_id != should_student_id) continue;
        return &(iter->all_workouts->at(iter->index)); //WARNING: this pointer will break after vector reallocation
    }
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
    std::vector<Workout_Info_> info(1, *workout_info);
    Workout_Hash_Container container = { &info, 0 };
    _real_hashes.erase(container);
    _last_real_hashes.erase(container);
    _should_hashes.erase(container);
    IM_ASSERT(workout_info - _all_workouts.data() == index);
    auto iter = _all_workouts.begin() + index;
    if (iter != _all_workouts.end()) _all_workouts.erase(iter);
}

bool Workout_Handler::change_lesson_info_position(int month, int wday, int old_merged_lesson_id,
    int new_merged_lesson_id, int max_merged_lessons_size)
{
    bool is_new = old_merged_lesson_id == -1;
    bool is_remove = new_merged_lesson_id == -1;
    IM_ASSERT(!(is_new && is_remove));
    if (old_merged_lesson_id == new_merged_lesson_id) return false;
    bool moved_right = new_merged_lesson_id > old_merged_lesson_id;
    int min_affected = moved_right ? old_merged_lesson_id : new_merged_lesson_id;
    int max_affected = moved_right ? new_merged_lesson_id : old_merged_lesson_id;
    if (is_new) // newly created lesson
    {
        min_affected = new_merged_lesson_id;
        max_affected = max_merged_lessons_size;
    } else if (is_remove)
    {
        min_affected = old_merged_lesson_id;
        max_affected = max_merged_lessons_size;
    }

    // check if can remove anything (for should_):
    for (const auto& workout : _all_workouts)
    {
        if (workout.should_attend.tm_wday != wday) continue;
        if (workout.should_attend.tm_mon != month) continue;
        if (workout.should_lesson.merged_lesson_id < min_affected) continue;
        if (workout.should_lesson.merged_lesson_id > max_affected) continue;
        int merged_lesson = workout.should_lesson.merged_lesson_id;
        if (is_remove && merged_lesson == min_affected) return false;
    }

    // check if can remove anything (for real_):
    for (const auto& workout : _all_workouts)
    {
        if (workout.real_attend.tm_wday != wday) continue;
        if (workout.real_attend.tm_mon != month) continue;
        if (workout.real_lesson.merged_lesson_id < min_affected) continue;
        if (workout.real_lesson.merged_lesson_id > max_affected) continue;
        int merged_lesson = workout.real_lesson.merged_lesson_id;
        if (is_remove && merged_lesson == min_affected) return false;
    }

    // fill replacement table for real_
    std::vector<std::pair<Workout_Info_, const Workout_Info_*>> new_and_to_remove;
    for (int merged_lesson = min_affected; merged_lesson <= max_affected; merged_lesson++)
    {
        bool decrement = moved_right && (merged_lesson != min_affected);
        bool increment = !moved_right && (merged_lesson != max_affected);
        bool to_max = moved_right && merged_lesson == min_affected;
        bool to_min = !moved_right && merged_lesson == max_affected;
        if (is_new)
        {
            decrement = false;
            increment = true;
            to_max = false;
            to_min = false;
        }
        else if (is_remove)
        {
            decrement = true;
            increment = false;
            to_max = false;
            to_min = false;
        }
        for (int internal_lesson = 0; internal_lesson < MAX_INTERNAL_LESSONS; internal_lesson++)
        {
            Lesson lesson = {.merged_lesson_id = merged_lesson, .internal_lesson_id = internal_lesson};
            auto info = get_info(month, wday, lesson);
            for (int i = 0; i < info.size(); i++)
            {
                if (is_remove && merged_lesson == min_affected) return false;
                for (int j = 0; j < info[i].size(); j++)
                {
                    if (info[i][j] == nullptr) continue;
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
    }

    for (auto info : new_and_to_remove)
    {
        insert_info(info.first);
    }

    new_and_to_remove.clear();

    // fill replacement table for should_
    for (const auto& workout : _all_workouts)
    {
        if (workout.should_attend.tm_wday != wday) continue;
        if (workout.should_attend.tm_mon != month) continue;
        if (workout.should_lesson.merged_lesson_id < min_affected) continue;
        if (workout.should_lesson.merged_lesson_id > max_affected) continue;
        int merged_lesson = workout.should_lesson.merged_lesson_id;
        if (is_remove && merged_lesson == min_affected) return false;
        bool decrement = moved_right && (merged_lesson != min_affected);
        bool increment = !moved_right && (merged_lesson != max_affected);
        bool to_max = moved_right && merged_lesson == min_affected;
        bool to_min = !moved_right && merged_lesson == max_affected;
        if (is_new)
        {
            decrement = false;
            increment = true;
            to_max = false;
            to_min = false;
        }
        else if (is_remove)
        {
            decrement = true;
            increment = false;
            to_max = false;
            to_min = false;
        }
        Workout_Info_ new_info = workout;

        if (decrement) new_info.should_lesson.merged_lesson_id--;
        else if (increment) new_info.should_lesson.merged_lesson_id++;
        else if (to_max) new_info.should_lesson.merged_lesson_id = max_affected;
        else if (to_min) new_info.should_lesson.merged_lesson_id = min_affected;

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

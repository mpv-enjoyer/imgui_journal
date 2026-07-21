#pragma once
#include "common/ptr.h"
#include "workouts.h"
#include "holidays.h"
#include "attendance_wdays.h"
#include "teachers.h"
#include "students.h"
#include "lesson_infos.h"

class Journal_Year
{
    Ptr<Workouts> m_workouts = Ptr<Workouts>::make();
    Ptr<Holidays> m_holidays = Ptr<Holidays>::make();
    Ptr<Attendance_Wdays> m_attendance_wdays = Ptr<Attendance_Wdays>::make();
    Ptr<Teachers> m_teachers = Ptr<Teachers>::make();
    Ptr<Students> m_students = Ptr<Students>::make();
    Ptr<Lesson_Infos> m_lesson_infos = Ptr<Lesson_Infos>::make();
    const Year m_bottom_year;
    
    struct Cache
    {
        // TODO: I'm free of any weirdness right?! Array initialization is weird.
        std::array<std::optional<Vector_Sortable_CIterator_Sorted<Attendance_Merged_Lesson>>, Wday::COUNT> sorted_merged_lessons = {};
        std::array<std::vector<std::optional<int>>, Month::COUNT> discounts_for_contracts = {};
        std::optional<std::vector<std::vector<std::pair<Merged_Lesson_ID, Position<Attendance_Student>>>>> merged_lessons_for_all_students;
    };
    mutable Cache m_cache;
public:
    explicit Journal_Year(Year bottom_year) : m_bottom_year(bottom_year)
    {
        
    }
    Mday get_mday(Wday wday, Aday aday) const { return Mday::make_from_aday(m_bottom_year, wday, aday); }
    size_t get_aday_count(Wday wday) const { return wday.calculate_count_for_bottom_year(m_bottom_year); }
    Month get_month_begin() const { return Month::make_begin_study_year_from_bottom_year(m_bottom_year); }
    bool is_within_current(Month month) const { return month.get_study_bottom_year() == m_bottom_year; }

    // cached:
    Vector_Sortable_CIterator_Sorted<Attendance_Merged_Lesson> get_merged_lessons_sorted(Wday wday) const
    {
        auto& cache = m_cache.sorted_merged_lessons[wday.get_EN()];
        if (!cache)
        {
            cache.emplace(attendance_wdays()->cref_wday(wday).cref_merged_lessons().csorted_begin());
        }
        return *cache;
    }
    int get_default_discount_id(Month month, Position<Contract> contract_pos)
    {
        auto discount_student_counter = [this](Month month, Position<Contract> contract) -> int
        {
            int student_counter = -1;
            for (const auto& student : students()->cref_students())
            {
                if (student.is_removed(month)) continue;
                if (contract == student.get_contract_pos()) student_counter++;
            }
            if (student_counter == -1) student_counter = 0; // Called by the deleted student?
            return student_counter;
        };
        auto discount_lesson_contract_counter = [this](Month month, Position<Contract> contract) -> int
        {
            auto& students = this->students()->cref_students();
            int lessons_contract_counter = -1;
            Wday wday = Wday::make_begin_EN();
            do
            {
                for (const auto& merged_lesson : attendance_wdays()->cref_wday(wday).cref_merged_lessons())
                {
                    if (merged_lesson.is_removed(month)) continue;
                    for (const auto& internal_lesson : merged_lesson.cref_internal_lessons())
                    {
                        for (auto student_it = internal_lesson.cref_students().begin(); student_it; ++student_it)
                        {
                            const auto& student = *student_it;
                            if (students[student.get_student_pos()].get_contract_pos() != contract) continue;
                            if (merged_lesson.is_student_removed(student_it.get_position(), month)) continue;
                            if (students[student.get_student_pos()].is_removed(month)) continue;
                            if (!student.get_wants_lesson()) continue;
                            lessons_contract_counter++;
                        }
                    }
                }
            } while (wday.next());
            if (lessons_contract_counter == -1) lessons_contract_counter = 0; // Called by the student with no lessons?
            lessons_contract_counter /= 2; // We want to have a number of full lessons
            return lessons_contract_counter;
        };
        auto& cache = m_cache.discounts_for_contracts[month.calculate_study_year_index()][contract_pos.get()];
        if (!cache)
        {
            int student_counter = discount_student_counter(month, contract_pos);
            int lesson_contract_counter = discount_lesson_contract_counter(month, contract_pos);
            if (student_counter > 0)
            {
                return std::max(student_counter, lesson_contract_counter);
            }
            static const int max_discount_for_single = 1;
            cache = std::min(lesson_contract_counter, max_discount_for_single);
        }
        return *cache;
    }
    std::vector<std::pair<Merged_Lesson_ID, Position<Attendance_Student>>> get_merged_lessons_for_student(Position<Student> student_pos) const
    {
        auto& cache = m_cache.merged_lessons_for_all_students;
        if (!cache)
        {
            cache = std::vector<std::vector<std::pair<Merged_Lesson_ID, Position<Attendance_Student>>>>(students()->cref_students().size());
            Wday wday = Wday::make_begin_RU();
            do
            {
                for (auto merged_lesson_it = cref_wday(wday).cref_merged_lessons().cbegin(); !!merged_lesson_it; ++merged_lesson_it)
                {
                    // Position<Attendance_Student> MUST be valid for all internal lessons inside a single Merged_Lesson, so
                    // it's fine to only iterate the first element
                    for (auto it = merged_lesson_it->cref_first_internal_lesson().cref_students().cbegin(); !!it; ++it)
                    {
                        (*cache)[it->get_student_pos().get()].emplace_back(Merged_Lesson_ID(wday, merged_lesson_it.get_position()), it.get_position());
                    }
                }
            } while (wday.next());
        }
        return (*cache)[student_pos.get()];
    }
    void cache_invalidate()
    {
        for (auto& per_wday : m_cache.sorted_merged_lessons)
        {
            per_wday.reset();
        }
        size_t contract_count = students()->cref_contracts().size();
        for (auto& per_month : m_cache.discounts_for_contracts)
        {
            per_month.clear();
            per_month = std::vector<std::optional<int>>(contract_count);
        }
        m_cache.merged_lessons_for_all_students.reset();
    }

    const Ptr<Workouts>& workouts() const { return m_workouts; }
          Ptr<Workouts>& workouts()       { return m_workouts; }
    const Ptr<Holidays>& holidays() const { return m_holidays; }
          Ptr<Holidays>& holidays()       { return m_holidays; }
    const Ptr<Attendance_Wdays>& attendance_wdays() const { return m_attendance_wdays; }
          Ptr<Attendance_Wdays>& attendance_wdays()       { return m_attendance_wdays; }
    const Ptr<Teachers>& teachers() const { return m_teachers; }
          Ptr<Teachers>& teachers()       { return m_teachers; }
    const Ptr<Students>& students() const { return m_students; }
          Ptr<Students>& students()       { return m_students; }
    const Ptr<Lesson_Infos>& lesson_infos() const { return m_lesson_infos; }
          Ptr<Lesson_Infos>& lesson_infos()       { return m_lesson_infos; }
    
    const Attendance_Wday& cref_wday(Wday wday) const
    {
        return attendance_wdays()->cref_wday(wday);
    }
    Attendance_Wday& ref_wday(Wday wday)
    {
        return attendance_wdays()->ref_wday(wday);
    }
    const Attendance_Merged_Lesson& cref_merged_lesson(Merged_Lesson_ID id) const
    {
        return cref_wday(id.wday()).cref_merged_lessons().cref(id.pos());
    }
    Attendance_Merged_Lesson& ref_merged_lesson(Merged_Lesson_ID id)
    {
        return ref_wday(id.wday()).ref_merged_lessons().ref(id.pos());
    }
    const Attendance_Internal_Lesson& cref_internal_lesson(Internal_Lesson_ID id) const
    {
        return cref_merged_lesson(id.merged_lesson_id()).cref_internal_lessons().cref(id.pos());
    }
    Attendance_Internal_Lesson& ref_internal_lesson(Internal_Lesson_ID id)
    {
        return ref_merged_lesson(id.merged_lesson_id()).ref_internal_lessons().ref(id.pos());
    }
    const Attendance_Student& cref_attendance_student(Internal_Student_ID id) const
    {
        return cref_internal_lesson(id.internal_lesson_id()).cref_student(id.pos());
    }
    Attendance_Student& ref_attendance_student(Internal_Student_ID id)
    {
        return ref_internal_lesson(id.internal_lesson_id()).ref_student(id.pos());
    }
    const Attendance_Holder& cref_attendance_holder(Attendance_ID id) const
    {
        return cref_attendance_student(id.internal_student_id()).cref_holder(id.aday());
    }
    Attendance_Holder& ref_attendance_holder(Attendance_ID id)
    {
        return ref_attendance_student(id.internal_student_id()).ref_holder(id.aday());
    }
};
#pragma once
#include "journal_year.h"

namespace Helpers
{
    int get_default_discount_id(const Journal_Year& journal_year, Position<Student> student_pos)
    {
        auto discount_student_counter = [&](Position<Contract> contract) -> int
        {
            int student_counter = -1;
            for (const auto& student : journal_year.students()->cref_students())
            {
                if (student.is_removed()) continue;
                if (contract == student.get_contract_pos()) student_counter++;
            }
            if (student_counter == -1) student_counter = 0; // Called by the deleted student?
            return student_counter;
        };
        auto discount_lesson_contract_counter = [&](Position<Contract> contract) -> int
        {
            auto& students = journal_year.students()->cref_students();
            int lessons_contract_counter = -1;
            Wday wday = Wday::make_begin_EN();
            do
            {
                for (const auto& merged_lesson : journal_year.attendance_wdays()->cref_wday(wday).cref_merged_lessons())
                {
                    if (merged_lesson.is_removed()) continue;
                    for (const auto& internal_lesson : merged_lesson.cref_internal_lessons())
                    {
                        for (const auto& student : internal_lesson.cref_students())
                        {
                            if (students[student.get_student_pos()].get_contract_pos() != contract) continue;
                            if (student.is_removed()) continue;
                            if (students[student.get_student_pos()].is_removed()) continue;
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
        auto contract_pos = journal_year.students()->cref_students()[student_pos].get_contract_pos();
        int student_counter = discount_student_counter(contract_pos);
        int lesson_contract_counter = discount_lesson_contract_counter(contract_pos);
        if (student_counter > 0)
        {
            return std::max(student_counter, lesson_contract_counter);
        }
        static const int max_discount_for_single = 1;
        return std::min(lesson_contract_counter, max_discount_for_single);
    }
}

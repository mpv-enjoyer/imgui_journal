#pragma once
#include "icommand.h"

class Add_Merged_Lesson : public ICommand
{
public:
    struct Request
    {
        Attendance_Internal_Lesson::Type type;
        JTime begin;
        JTime end;
    };
private:
    Wday m_wday;
    int m_number;
    std::string m_comment;
    int m_age_group;
    std::vector<Request> m_lessons;
public:
    Add_Merged_Lesson(Wday wday, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : m_wday(wday), m_number(number), m_comment(comment), m_age_group(age_group), m_lessons(lessons)
    { }
    std::optional<std::string> get_error(const IModel& model) override
    {
        if (m_lessons.size() == 0 || m_lessons.size() > 2)
        {
            return "invalid m_lessons.size()";
        }
        auto& merged_lessons = model->attendance_wdays()->cref_wday(m_wday).cref_merged_lessons();
        for (auto it = merged_lessons.cbegin(); it; it.next())
        {
            if (it->get_number() == m_number)
            {
                return "Группа с таким номером в " + m_wday.get_name() + " уже существует";
            }
        }
        if (m_lessons.size() == 2)
        {
            bool insane_time = false;
            insane_time = insane_time || m_lessons[0].begin >= m_lessons[0].end;
            insane_time = insane_time || m_lessons[0].end   >  m_lessons[1].begin;
            insane_time = insane_time || m_lessons[1].begin >= m_lessons[1].end;
            if (insane_time) return "Недопустимое время";
        }
        else if (m_lessons[0].begin >= m_lessons[0].end)
        {
            return "Недопустимое время";
        }
        return {};
    }

    void call(IModel& model) override
    {
        std::vector<Ptr<Attendance_Internal_Lesson>> internal_lessons;
        for (auto lesson : m_lessons)
        {
            internal_lessons.push_back(Ptr<Attendance_Internal_Lesson>::make(lesson.type, lesson.begin, lesson.end));
        }
        model->attendance_wdays()
            ->ref_wday(m_wday)
            .ref_merged_lessons()
            .push_back(Ptr<Attendance_Merged_Lesson>::make(
                std::move(internal_lessons), m_number, m_age_group
            ));
    }
};
#pragma once
#include "icommand.h"

class Add_Or_Edit_Merged_Lesson : public ICommand
{
public:
    struct Request
    {
        Attendance_Internal_Lesson::Type type;
        JTime begin;
        JTime end;
    };
private:
    const std::optional<Position<Attendance_Merged_Lesson>> m_position;
    Wday m_wday;
    int m_number;
    std::string m_comment;
    int m_age_group;
    std::vector<Request> m_lessons;
public:
    Add_Or_Edit_Merged_Lesson(Wday wday, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : m_wday(wday), m_number(number), m_comment(comment), m_age_group(age_group), m_lessons(lessons)
    { }
    Add_Or_Edit_Merged_Lesson(Position<Attendance_Merged_Lesson> pos, Wday wday, int number, std::string comment, int age_group, std::vector<std::pair<JTime, JTime>> lessons)
    : m_position(pos), m_wday(wday), m_number(number), m_comment(comment), m_age_group(age_group)
    {
        for (auto lesson : m_lessons)
        {
            m_lessons.emplace_back(Request{.begin = lesson.begin, .end = lesson.end});
            // Not filling in type because you should not be able to edit it.
        }
    }
    Error get_error(const IModel& model) override
    {
        if (m_lessons.size() == 0 || m_lessons.size() > 2)
        {
            return "Неверный размер пары (0 или >2)";
        }
        auto& merged_lessons = model->attendance_wdays()->cref_wday(m_wday).cref_merged_lessons();
        if (m_position)
        {
            if (merged_lessons[*m_position].cref_internal_lessons().size() != m_lessons.size())
            {
                return "Количество уроков в паре не может быть изменено";
            }
        }
        for (auto it = merged_lessons.cbegin(); it; it.next())
        {
            if (it->is_removed()) continue;
            if (it->get_number() == m_number && !(m_position && it.get_position() == *m_position))
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
        auto& merged = model->attendance_wdays()->ref_wday(m_wday).ref_merged_lessons();
        if (m_position)
        {
            auto& current = merged.ref(*m_position);
            current.set_age_group(m_age_group);
            current.set_number(m_number);
            current.set_comment(m_comment);
            auto it = current.ref_internal_lessons().begin();
            do
            {
                size_t i = it.get_position().get();
                it->set_time(m_lessons[i].begin, m_lessons[i].end);
            } while (it.next());
            return;
        }

        std::vector<Ptr<Attendance_Internal_Lesson>> internal_lessons;
        for (auto lesson : m_lessons)
        {
            internal_lessons.push_back(Ptr<Attendance_Internal_Lesson>::make(lesson.type, lesson.begin, lesson.end));
        }
        merged.push_back(
            Ptr<Attendance_Merged_Lesson>::make(
                std::move(internal_lessons), m_number, m_age_group, m_comment));
    }
};
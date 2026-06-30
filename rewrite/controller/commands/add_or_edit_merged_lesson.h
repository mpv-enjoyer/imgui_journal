#pragma once
#include "icommand.h"

class Helper__Add_Or_Edit_Merged_Lesson : public ICommand
{
public:
    struct Request
    {
        Attendance_Internal_Lesson::Type type;
        JTime begin;
        JTime end;
    };
private:
    const Month M_MONTH;
    const std::optional<Position<Attendance_Merged_Lesson>> m_position;
    Wday m_wday;
    int m_number;
    std::string m_comment;
    int m_age_group;
    std::vector<Request> m_lessons;
public:
    Helper__Add_Or_Edit_Merged_Lesson(Wday wday, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : M_MONTH(Month::make_current()), m_wday(wday), m_number(number), m_comment(comment), m_age_group(age_group), m_lessons(lessons)
    { }
    Helper__Add_Or_Edit_Merged_Lesson(Month month, Merged_Lesson_ID id, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : M_MONTH(month), m_position(id.pos()), m_wday(id.wday()), m_number(number), m_comment(comment), m_age_group(age_group), m_lessons(lessons)
    { }

    Error get_error(const IModel& model) const override
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
        for (auto it = merged_lessons.cbegin(); !!it; ++it)
        {
            if (it->is_removed(M_MONTH)) continue;
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
        auto& merged = model->ref_wday(m_wday).ref_merged_lessons();
        if (m_position)
        {
            auto& current = merged.ref(*m_position);
            // TODO: decide whether this is per_month
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

        std::size_t adays_count = model->get_aday_count(m_wday);
        std::vector<Ptr<Attendance_Internal_Lesson>> internal_lessons;
        for (auto lesson : m_lessons)
        {
            internal_lessons.push_back(Ptr<Attendance_Internal_Lesson>::make(adays_count, lesson.type, lesson.begin, lesson.end));
        }
        
        merged.push_back(
            Ptr<Attendance_Merged_Lesson>::make(
                std::move(internal_lessons), m_number, m_age_group, m_comment));
    }
};

class Add_Merged_Lesson : public Helper__Add_Or_Edit_Merged_Lesson
{
public:
    Add_Merged_Lesson(Wday wday, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : Helper__Add_Or_Edit_Merged_Lesson(wday, number, comment, age_group, lessons)
    { }
    CMD_WANT_STATE(Time_State::CurrentMonth);
};

class Edit_Merged_Lesson : public Helper__Add_Or_Edit_Merged_Lesson
{
public:
    Edit_Merged_Lesson(Month month, Merged_Lesson_ID id, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : Helper__Add_Or_Edit_Merged_Lesson(month, id, number, comment, age_group, lessons)
    { }
    CMD_WANT_STATE(Time_State::CurrentYear);
};

using Add_Or_Edit_Merged_Lesson_Request = Helper__Add_Or_Edit_Merged_Lesson::Request;
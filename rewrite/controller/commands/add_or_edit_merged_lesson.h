#pragma once
#include "icommand.h"

class Add_Or_Edit_Merged_Lesson : public ICommand
{
public:
    struct Request
    {
        AInternal_Lesson::Type type;
        JTime begin;
        JTime end;
    };
private:
    const std::optional<Pos<AMerged_Lesson>> m_position;
    std::vector<bool> m_adays_are_active;
    Wday m_wday;
    int m_number;
    std::string m_comment;
    int m_age_group;
    std::vector<Request> m_lessons;
public:
    Add_Or_Edit_Merged_Lesson(Mday mday, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : m_wday(Wday::make_from_mday(mday)), m_number(number), m_comment(comment), m_age_group(age_group), m_lessons(lessons)
    {
        Year bottom_year = mday.get_month().get_study_bottom_year();
        std::size_t adays_count = m_wday.calculate_count_for_bottom_year(bottom_year);
        std::size_t requested_aday_index = Aday::make_from_mday(mday).index();
        for (size_t i = 0; i < adays_count; i++)
        {
            m_adays_are_active.push_back(i >= requested_aday_index);
        }
    }
    Add_Or_Edit_Merged_Lesson(Merged_Lesson_ID id, std::vector<bool> adays_are_active, int number, std::string comment, int age_group, std::vector<Request> lessons)
    : m_position(id.pos()), m_adays_are_active(adays_are_active), m_wday(id.wday()), m_number(number), m_comment(comment), m_age_group(age_group), m_lessons(lessons)
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
        for (auto it = merged_lessons.cbegin(); it; it.next())
        {
            if (it->is_removed()) continue;
            if (it->get_number() == m_number && !(m_position && it.get_pos() == *m_position))
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
            current.set_age_group(m_age_group);
            current.set_number(m_number);
            current.set_comment(m_comment);
            auto it = current.ref_internal_lessons().begin();
            do
            {
                size_t i = it.get_pos().get();
                it->set_time(m_lessons[i].begin, m_lessons[i].end);
            } while (it.next());
            current.set_active_adays(m_adays_are_active);
            return;
        }

        std::size_t adays_count = m_adays_are_active.size();
        std::vector<Ptr<AInternal_Lesson>> internal_lessons;
        for (auto lesson : m_lessons)
        {
            internal_lessons.push_back(Ptr<AInternal_Lesson>::make(adays_count, lesson.type, lesson.begin, lesson.end));
        }

        std::vector<Aday_With_Status> adays_with_status;
        for (size_t index = 0; index < adays_count; index++)
        {
            Aday_With_Status current =
            {
                .aday = Aday::make_from_index(index),
                .is_active = m_adays_are_active[index]
            };
            adays_with_status.push_back(current);
        }

        merged.push_back(
            Ptr<AMerged_Lesson>::make(
                std::move(internal_lessons), m_number, m_age_group, m_comment, adays_with_status));
    }
};
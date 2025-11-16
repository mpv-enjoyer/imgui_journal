#pragma once
#include "icommand.h"

class Remove_Lesson : public ICommand
{
    Merged_Lesson_ID m_id;
public:
    Remove_Lesson(Merged_Lesson_ID id)
    : m_id(id)
    { }
    Error get_error(const IModel& model) override
    {
        if (model->attendance_wdays()->cref_merged_lesson(m_id).is_removed()) return "Группа уже удалена";
        return {};
    }
    void call(IModel& model) override
    {
        model->attendance_wdays()->ref_merged_lesson(m_id).remove();
    }
};

class Restore_Lesson : public ICommand
{
    Merged_Lesson_ID m_id;
public:
    Restore_Lesson(Merged_Lesson_ID id)
    : m_id(id)
    { }
    Error get_error(const IModel& model) override
    {
        if (!model->attendance_wdays()->cref_merged_lesson(m_id).is_removed()) return "Группа не удалена";
        return {};
    }
    void call(IModel& model) override
    {
        model->attendance_wdays()->ref_merged_lesson(m_id).restore();
    }
};

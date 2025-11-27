#pragma once
#include "icommand.h"

class Remove_Lesson : public ICommand
{
    Merged_Lesson_ID m_id;
public:
    Remove_Lesson(Merged_Lesson_ID id)
    : m_id(id)
    { }
    Error get_error(const IModel& model) const override
    {
        if (model->cref_merged_lesson(m_id).is_removed()) return "Группа уже удалена";
        return {};
    }
    void call(IModel& model) override
    {
        model->ref_merged_lesson(m_id).remove();
    }
};

class Restore_Lesson : public ICommand
{
    Merged_Lesson_ID m_id;
public:
    Restore_Lesson(Merged_Lesson_ID id)
    : m_id(id)
    { }
    Error get_error(const IModel& model) const override
    {
        if (!model->cref_merged_lesson(m_id).is_removed()) return "Группа не удалена";
        return {};
    }
    void call(IModel& model) override
    {
        model->ref_merged_lesson(m_id).restore();
    }
};

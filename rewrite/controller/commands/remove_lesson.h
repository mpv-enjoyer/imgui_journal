#pragma once
#include "icommand.h"

class Remove_Lesson : public ICommand
{
    Merged_Lesson_ID m_id;
public:
    Remove_Lesson(Merged_Lesson_ID id)
    : m_id(id)
    { }

    CMD_WANT_STATE(Time_State::CurrentMonth)

    Error get_error(const IModel& model) const override
    {
        if (model->cref_merged_lesson(m_id).is_removed(Month::make_current())) return "Группа уже удалена";
        return {};
    }
    void call(IModel& model) override
    {
        model->ref_merged_lesson(m_id).remove(Month::make_current());
    }
};

class Restore_Lesson : public ICommand
{
    Merged_Lesson_ID m_id;
public:
    Restore_Lesson(Merged_Lesson_ID id)
    : m_id(id)
    { }

    CMD_WANT_STATE(Time_State::CurrentMonth)

    Error get_error(const IModel& model) const override
    {
        if (!model->cref_merged_lesson(m_id).is_removed(Month::make_current())) return "Группа не удалена";
        return {};
    }
    void call(IModel& model) override
    {
        model->ref_merged_lesson(m_id).restore(Month::make_current());
    }
};

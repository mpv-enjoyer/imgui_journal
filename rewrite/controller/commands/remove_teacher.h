#pragma once
#include "icommand.h"
#include "add_or_edit_teacher.h"

class Remove_Teacher : public ICommand
{
    Position<Teacher> m_teacher_pos;
public:
    Remove_Teacher(Position<Teacher> teacher_pos)
    : m_teacher_pos(teacher_pos)
    { }
    Error get_error(const IModel& model) const override
    {
        if (model->teachers()->cref_data()[m_teacher_pos].is_removed()) return "Учитель уже удален";
        return {};
    }
    void call(IModel& model) override
    {
        const Teacher& teacher = model->teachers()->cref_data()[m_teacher_pos];
        Add_Or_Edit_Teacher(m_teacher_pos, teacher.get_name() + " [-]", teacher.get_abbreviation());
        model->teachers()->ref_data()[m_teacher_pos].remove();
    }
};

class Restore_Teacher : public ICommand
{
    Position<Teacher> m_teacher_pos;
public:
    Restore_Teacher(Position<Teacher> teacher_pos)
    : m_teacher_pos(teacher_pos)
    { }
    Error get_error(const IModel& model) const override
    {
        if (!model->teachers()->cref_data()[m_teacher_pos].is_removed()) return "Учитель не удален";
        return {};
    }
    void call(IModel& model) override
    {
        model->teachers()->ref_data()[m_teacher_pos].restore();
    }
};
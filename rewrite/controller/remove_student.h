#pragma once
#include "icommand.h"
#include "add_or_edit_student_in_base.h"

class Remove_Student : public ICommand
{
    Position<Student> m_student_pos;
public:
    Remove_Student(Position<Student> student_pos)
    : m_student_pos(student_pos)
    { }
    Error get_error(const IModel& model) override
    {
        if (model->students()->cref_students().cref(m_student_pos).cref_removal_info().is_removed()) return "Ученик уже удален";
        return {};
    }
    void call(IModel& model) override
    {
        const Student& student = model->students()->cref_students().cref(m_student_pos);
        Add_Or_Edit_Student_In_Base(m_student_pos, student.get_name() + " [-]", student.get_contract_pos().get()).call(model);
        model->students()->ref_removal_info(m_student_pos).remove();
    }
};

class Restore_Student : public ICommand
{
    Position<Student> m_student_pos;
public:
    Restore_Student(Position<Student> student_pos)
    : m_student_pos(student_pos)
    { }
    Error get_error(const IModel& model) override
    {
        if (!model->students()->cref_students().cref(m_student_pos).cref_removal_info().is_removed()) return "Ученик не удален";
        return {};
    }
    void call(IModel& model) override
    {
        model->students()->ref_removal_info(m_student_pos).restore();
    }
};

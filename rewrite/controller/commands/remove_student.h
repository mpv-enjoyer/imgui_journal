#pragma once
#include "icommand.h"
#include "add_or_edit_student_in_base.h"

class Remove_Student : public ICommand
{
    Pos<Student> m_student_pos;
public:
    Remove_Student(Pos<Student> student_pos)
    : m_student_pos(student_pos)
    { }
    Error get_error(const IModel& model) const override
    {
        if (model->students()->cref_students().cref(m_student_pos).is_removed()) return "Ученик уже удален";
        return {};
    }
    void call(IModel& model) override
    {
        const Student& student = model->students()->cref_students()[m_student_pos];
        Add_Or_Edit_Student_In_Base(m_student_pos, student.get_name() + " [-]", model->students()->get_contract_number(m_student_pos)).call(model);
        model->students()->ref_removal_info(m_student_pos).remove();
    }
};

class Restore_Student : public ICommand
{
    Pos<Student> m_student_pos;
public:
    Restore_Student(Pos<Student> student_pos)
    : m_student_pos(student_pos)
    { }
    Error get_error(const IModel& model) const override
    {
        if (!model->students()->cref_students()[m_student_pos].is_removed()) return "Ученик не удален";
        return {};
    }
    void call(IModel& model) override
    {
        model->students()->ref_removal_info(m_student_pos).restore();
    }
};

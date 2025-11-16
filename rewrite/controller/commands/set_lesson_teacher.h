#pragma once
#include "icommand.h"

class Set_Lesson_Teacher : public ICommand
{
    Internal_Lesson_ID m_internal_lesson_id;
    Aday m_aday;
    Position<Teacher> m_teacher_pos;
public:
    Set_Lesson_Teacher(Internal_Lesson_ID internal_lesson_id, Aday aday, Position<Teacher> teacher_pos)
    : m_internal_lesson_id(internal_lesson_id), m_aday(aday), m_teacher_pos(teacher_pos) { }
    Error get_error(const IModel&) override
    {
        return {};
    }
    void call(IModel& model)
    {
        auto& internal_lesson = model->attendance_wdays()->ref_internal_lesson(m_internal_lesson_id);
        internal_lesson.set_teacher_pos(m_aday, m_teacher_pos);
    }
};

class Reset_Lesson_Teacher : public ICommand
{
    Internal_Lesson_ID m_internal_lesson_id;
    Aday m_aday;
public:
    Reset_Lesson_Teacher(Internal_Lesson_ID internal_lesson_id, Aday aday)
    : m_internal_lesson_id(internal_lesson_id), m_aday(aday) { }
    Error get_error(const IModel&) override
    {
        return {};
    }
    void call(IModel& model)
    {
        auto& internal_lesson = model->attendance_wdays()->ref_internal_lesson(m_internal_lesson_id);
        internal_lesson.reset_teacher_pos(m_aday);
    }
};
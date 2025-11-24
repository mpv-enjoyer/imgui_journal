#pragma once
#include "icommand.h"

class Set_Lesson_Comment : public ICommand
{
    Internal_Lesson_ID m_internal_lesson_id;
    Aday m_aday;
    std::string m_comment;
public:
    Set_Lesson_Comment(Internal_Lesson_ID internal_lesson_id, Aday aday, std::string comment)
    : m_internal_lesson_id(internal_lesson_id), m_aday(aday), m_comment(comment) { }
    Error get_error(const IModel&) const override
    {
        return {};
    }
    void call(IModel& model)
    {
        auto& internal_lesson = model->attendance_wdays()->ref_internal_lesson(m_internal_lesson_id);
        internal_lesson.set_comment(m_aday, m_comment);
    }
};
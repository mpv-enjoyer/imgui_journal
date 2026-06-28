#pragma once
#include "icommand.h"

class Add_Or_Edit_Teacher : public ICommand
{
    std::optional<Position<Teacher>> m_teacher_pos;
    std::string m_name;
    std::string m_abbreviation;
public:
    Add_Or_Edit_Teacher(Position<Teacher> teacher_pos, std::string name, std::string abbreviation)
    : m_teacher_pos(teacher_pos), m_name(name), m_abbreviation(abbreviation) { }
    Add_Or_Edit_Teacher(std::string name, std::string abbreviation)
    : m_name(name), m_abbreviation(abbreviation) { }
    Error get_error(const IModel& model) const override
    {
        return {};
    }
    void call(IModel& model) override
    {
        if (!m_teacher_pos)
        {
            model->teachers()->ref_data().push_back(Ptr<Teacher>::make(m_name, m_abbreviation));
        }
        else
        {
            model->teachers()->ref_data()[*m_teacher_pos].set_name(m_name);
            model->teachers()->ref_data()[*m_teacher_pos].set_abbreviation(m_abbreviation);
        }
    }
};
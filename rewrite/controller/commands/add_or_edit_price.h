#pragma once
#include "icommand.h"

class Add_Or_Edit_Price : public ICommand
{
    Lesson_Type m_lesson_type;
    Month m_month;
    Price m_price;
    std::optional<std::size_t> m_discount_id;
public:
    Add_Or_Edit_Price(Lesson_Type lesson_type, Month month, Price price)
    : m_lesson_type(lesson_type), m_month(month), m_price(price)
    { }
    Add_Or_Edit_Price(Lesson_Type lesson_type, Month month, Price price, std::size_t discount_id)
    : m_lesson_type(lesson_type), m_month(month), m_price(price), m_discount_id(discount_id)
    { }

    CMD_WANT_STATE(Time_State::CurrentYear)

    Error get_error(const IModel&) const override
    {
        if (m_price.get() < 0) return "Невозможно установить негативную цену";
        return {};
    }
    void call(IModel& model) override
    {
        if (m_discount_id) model->lesson_infos()->set_price(m_lesson_type, m_month, *m_discount_id, m_price);
        else model->lesson_infos()->add_price(m_lesson_type, m_month, m_price);
    }
};

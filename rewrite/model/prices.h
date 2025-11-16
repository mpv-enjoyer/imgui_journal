#pragma once
#include "attendance_internal_lesson.h"
#include "price.h"

class Prices
{
    std::vector<std::vector<Lesson_Type_Prices>> m_lesson_prices =
    {{{
        .drawing = Price::Value_Type(1),
        .sculpting = Price::Value_Type(2),
        .design = Price::Value_Type(3),
        .techdrawing = Price::Value_Type(4),
        .specialcourse = Price::Value_Type(5)
    }}};
public:
    Prices()
    {
        TODO_CRITICAL("Replace Lesson_Type_Prices and Lesson_Type enum with a simple std::array");
    }
    Lesson_Type_Prices get_prices(Month month, Lesson_Type lesson_type, size_t discount_id)
    {
        IM_ASSERT(m_lesson_prices.size() > 0);
        IM_ASSERT(m_lesson_prices[0].size() > 0);
        DEBUG_ASSERT(discount_id != -1);
        if (discount_id >= discount_size) discount_id = discount_size - 1;
        switch (lesson_type)
        {
        case Lesson_Type::DRAWING: return m_lesson_prices[0][discount_id];
        case Lesson_Type::SCULPTING: return m_lesson_prices[1][discount_id];
        case Lesson_Type::DESIGN: return m_lesson_prices[2][discount_id];
        case Lesson_Type::TECHDRAWING: return m_lesson_prices[3][discount_id];
        case Lesson_Type::SPECIALCOURSE: return m_lesson_prices[4][discount_id];
        default: IM_ASSERT(false);
        }
    }
};
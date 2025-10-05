#pragma once
#include "attendance_internal_lesson.h"
#include "price.h"

class Prices
{
    std::vector<std::vector<Price>> m_lesson_prices;
    int discount_size = 0;
public:
    Price get_price(Lesson_Type lesson_type, int discount_id)
    {
        IM_ASSERT(m_lesson_prices.size() > 0);
        IM_ASSERT(m_lesson_prices[0].size() > 0);
        IM_ASSERT(discount_id != -1);
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
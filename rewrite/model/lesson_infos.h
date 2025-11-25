#pragma once
#include <cstdint>
#include <vector>
#include "jtime.h"
#include "price.h"

class Lesson_Infos
{
public:
    enum class Type
    {
        DRAWING = 0,        // ИЗО
        SCULPTING = 1,      // Лепка
        DESIGN = 2,         // Дизайн
        TECHDRAWING = 3,    // Черчение
        SPECIALCOURSE = 4,  // Спецкурс
    };
    const static std::size_t ID_COUNT = 5;
private:
    struct Lesson_Info
    {
        std::vector<std::vector<Price>> prices; // [month_from_study_year][discount_id]
        Lesson_Info(Price default_price_ = Price::Value_Type(1))
        : prices(Month::COUNT, std::vector<Price>(1, default_price_))
        { }
        std::vector<Price>& price(Month month) { return prices[month.calculate_study_year_index()]; }
        const std::vector<Price>& price(Month month) const { return prices[month.calculate_study_year_index()]; }
    };
    std::vector<Lesson_Info> m_data;
    static std::size_t translate_lesson_type(Type type)
    {
        switch (type)
        {
        case Type::DRAWING: return 0;       // ИЗО
        case Type::SCULPTING: return 1;     // Лепка
        case Type::DESIGN: return 2;        // Дизайн
        case Type::TECHDRAWING: return 3;   // Черчение
        case Type::SPECIALCOURSE: return 4; // Спецкурс
        }
        IM_ASSERT(false && "unreachable");
    }
    Lesson_Info& lesson_info(Type type) { return m_data[translate_lesson_type(type)]; }
    const Lesson_Info& lesson_info(Type type) const { return m_data[translate_lesson_type(type)]; }
public:
    Lesson_Infos()
    {
        for (size_t i = 0; i < ID_COUNT; i++)
        {
            m_data.push_back(Lesson_Info());
        }
    }
    Price get_price(Type lesson_type, Month month, std::size_t discount_id) const
    {
        const auto& current = lesson_info(lesson_type).price(month);
        if (discount_id >= current.size()) return current.back();
        return current[discount_id];
    }
    static std::string get_name(Type lesson_type)
    {
        static std::array<std::string, ID_COUNT> NAMES =
        {
            "ИЗО",
            "Лепка",
            "Дизайн",
            "Черчение",
            "Спецкурс", 
        };
        return NAMES[translate_lesson_type(lesson_type)];
    }
    size_t get_discounts_count(Type lesson_type, Month month) const
    {
        return lesson_info(lesson_type).price(month).size();
    }
    void add_price(Type lesson_type, Month month, Price price)
    {
        lesson_info(lesson_type).price(month).push_back(price);
    }
    void set_price(Type lesson_type, Month month, std::size_t discount_id, Price price)
    {
        lesson_info(lesson_type).price(month)[discount_id] = price;
    }
};

using Lesson_Type = Lesson_Infos::Type;
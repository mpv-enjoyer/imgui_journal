#pragma once

#include "../main_types/main_types.h"
#include "../helpers/helpers.h"

class Prices {
    std::vector<std::vector<int>> _lesson_prices = 
        {{100, 99, 98},
        {200, 199, 198}, 
        {300, 299, 298}, 
        {400, 399, 398}, 
        {500, 499, 498},
        {600, 599, 598}};
    int _lesson_price_skipped = 50;
    int _lesson_price_was_ill = 0;

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version)
    {
        ar << _lesson_prices;
        ar << _lesson_price_skipped;
        ar << _lesson_price_was_ill;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar >> _lesson_prices;
        fixup_missing_prices(_lesson_prices);
        validate(_lesson_prices);
        ar >> _lesson_price_skipped;
        ar >> _lesson_price_was_ill;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    static void fixup_missing_prices(std::vector<std::vector<int>> &prices)
    {
        if (prices.size() == LESSON_TYPE_COUNT)
        {
            /* everything good */
        }
        else if (prices.size() < LESSON_TYPE_COUNT)
        {
            int discounts_size = prices[0].size();
            // Пленэр?
            for (int i_ = prices.size(); i_ < LESSON_TYPE_COUNT; i_++)
            {
                prices.push_back({});
                for (int discount_id = 0; discount_id < discounts_size; discount_id++)
                {
                    prices.back().push_back(999);
                }
            }
        }
        else
        {
            IM_ASSERT(false && "Prices: Found more lesson_types than supported");
        }
    }

    static void validate(const std::vector<std::vector<int>>& prices)
    {
        IM_ASSERT(prices.size() == LESSON_TYPE_COUNT);
        int discount_count = prices[0].size();
        for (int i = 1; i < prices.size(); i++) {
            IM_ASSERT(discount_count == prices[i].size());
        }
    }
public:
    const std::vector<std::vector<int>>& get_lesson_prices() { return _lesson_prices; }
    int get_ill_price() { return _lesson_price_was_ill; }
    int get_skipped_price() { return _lesson_price_skipped; }
    void set_prices(std::vector<std::vector<int>> prices, int ill_price, int skipped_price)
    {
        validate(prices);
        _lesson_prices = prices;
        _lesson_price_was_ill = ill_price;
        _lesson_price_skipped = skipped_price;
    }
    void get_prices(std::vector<std::vector<int>>& prices, int& ill_price, int& skipped_price)
    {
        prices = _lesson_prices;
        ill_price = _lesson_price_was_ill;
        skipped_price = _lesson_price_skipped;
    }
};
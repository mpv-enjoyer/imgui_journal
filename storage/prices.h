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
};
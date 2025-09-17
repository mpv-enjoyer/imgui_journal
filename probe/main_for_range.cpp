#include <iostream>
#include "../rewrite/common/vector_sortable.h"

void i_want_const(const Vector_Sortable<int>& vector)
{
    for (const auto& elem : vector)
    {
        
    }
}

int main()
{
    Vector_Sortable<int> vector;
    vector.push_back(Ptr<int>::make(2));
    vector.push_back(Ptr<int>::make(2));
    vector.push_back(Ptr<int>::make(2));
    vector.push_back(Ptr<int>::make(2));
    for (auto elem : vector)
    {
        std::cout << elem;
    }
    std::cout << std::any_of(vector.begin(), vector.end(), [](int value) { return value > 4; });
}
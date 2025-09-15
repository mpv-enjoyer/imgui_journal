#include "../rewrite/common/vector_sortable.h"
#include <iostream>

int main()
{
    Vector_Sortable<int> ints;
    ints.push_back(Ptr<int>::make(1));
    ints.push_back(Ptr<int>::make(2));
    ints.push_back(Ptr<int>::make(3));
    for (auto it = ints.cbegin(); it; it.next())
    {
        std::cout << it.get() << "\n";
    }
}
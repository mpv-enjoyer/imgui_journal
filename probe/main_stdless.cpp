#include <functional>
#include <iostream>

struct S
{
    int i;
    S(int i_) : i(i_) { };
    bool operator<(const S& other) { return i < other.i; }
};

int main() {
    std::cout << std::less{}(S(1), S(2));
    S s1(1);
    S s2(2);
    S* sminptr = std::min(&s1, &s2);
    S* smaxptr = std::max(&s1, &s2);
    sminptr->i = 2;
    smaxptr->i = 1;
    std::cout << std::less{}(sminptr, smaxptr);
}
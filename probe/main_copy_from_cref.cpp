#include <string>

struct S
{
    int some_int = 2;
    const std::string some_string = "lol";
public:
    S(int i, std::string s) : some_int(i), some_string(s) { };
};

const S& get_stored_s()
{
    static const S s(3, "444");
    return s;
}

int main()
{
    S s2 = get_stored_s();
    return s2.some_int;
}
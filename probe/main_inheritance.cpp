struct S
{
    int s;
    S(int s_) : s(s_) { } 
};

struct S2 : private S
{
    int s2 = 4;
    S2() : S(1) { }
};

void i_want_s(S& s)
{

}

int main()
{
    S s{1};
    S2 s2;
    i_want_s(s2);
}
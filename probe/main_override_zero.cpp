struct S
{
    int g = 0;
    virtual void update() = 0;
};

struct S_Impl : S
{
    virtual void update() override { g = 1; }
};

struct S_Impl_Reimpl : S_Impl
{
    virtual void update() override = 0;
};

struct S_Impl_Reimpl_Reimpl : S_Impl_Reimpl
{
    virtual void update() override { g = 2; }
};

int main()
{
    S_Impl_Reimpl_Reimpl s;
    s.update();
    S_Impl* a = new S_Impl();
    return s.g;
}
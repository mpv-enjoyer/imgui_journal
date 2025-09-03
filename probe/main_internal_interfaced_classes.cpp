class IS
{
public:
    virtual void print() = 0;
};

#include <iostream>

IS& get_IS_impl()
{
    class IS_impl : public IS
    {
        void print()
        {
            std::cout << "Hello from IS_impl\n";
        }
    };
    return *(new IS_impl());
}

int main()
{
    auto& i = get_IS_impl();
    i.print();
}
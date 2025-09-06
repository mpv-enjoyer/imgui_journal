#include <memory>

// https://stackoverflow.com/a/46400534
template <typename T>
struct Ptr : std::unique_ptr<T> {
    using base_t = T;
    using std::unique_ptr<T>::unique_ptr;
    using std::unique_ptr<T>::operator =;

    const T *get() const noexcept {
        return std::unique_ptr<T>::get();
    }
    T *get() noexcept {
        return std::unique_ptr<T>::get();
    }

    const T &operator *() const noexcept {
        return std::unique_ptr<T>::operator *();
    }
    T &operator *() noexcept {
        return std::unique_ptr<T>::operator *();
    }

    const T *operator -> () const noexcept {
        return std::unique_ptr<T>::get();
    }
    T *operator -> () noexcept {
        return std::unique_ptr<T>::get();
    }

    template <typename... Args>
    static Ptr<T> make(Args&&... args)
    { return Ptr<T>(new T(std::forward<Args>(args)...)); }

    Ptr() = delete;
};

struct IS
{
    virtual void print() = 0;
};

#include <iostream>

struct S : public IS
{
    void print() override
    {
        std::cout << "Printing S\n";
    }
};

void print_whatever_IS(Ptr<IS> is_ptr)
{
    is_ptr->print();
}

struct Ptr_Holder
{
    Ptr<int> ptr;
    Ptr_Holder(Ptr<int> ptr_) : ptr(ptr_) { }
};

int main()
{
    print_whatever_IS(Ptr<S>::make());
    Ptr<int> f = Ptr<int>::make();
    //Ptr<S> s;
    Ptr_Holder ptr_holder = Ptr_Holder(Ptr<int>::make());
}
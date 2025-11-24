#pragma once
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

    //void swap(Ptr<T> &ptr) noexcept {
    //    static_cast<std::unique_ptr<T>>(*this).swap(static_cast<std::unique_ptr<T>>(ptr));
    //}

    template <typename... Args>
    static Ptr<T> make(Args&&... args)
    { return Ptr<T>(new T(std::forward<Args>(args)...)); }

    Ptr() = delete;
};
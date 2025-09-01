#pragma once
#include <functional>

template<typename ThisType, typename FuncType, typename... Args>
std::remove_const<decltype(FuncType(std::declval<Args...>()))> AUTOMUT(ThisType this_, FuncType const_func, Args... args)
{
    using ReturnValue = decltype(FuncType(std::declval<Args...>()));
    return const_cast<ReturnValue&>( static_cast<const ThisType&>(*this_).cfunc(args));
}
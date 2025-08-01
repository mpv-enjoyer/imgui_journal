#pragma once
#include <utility>

// https://stackoverflow.com/a/123765
template <typename ReturnValue, typename ThisType, typename... Args>
ReturnValue& doAutomut(const ThisType* this_, const ReturnValue& (ThisType::*func)(Args...) const, Args&&... args)
{
    return const_cast<ReturnValue&>((this_->*func)(std::forward<Args>(args)...));
}

#define AUTOMUT0(METHOD)                        auto& METHOD() { return doAutomut(this, &std::remove_reference_t<decltype(*this)>::c##METHOD); }
#define AUTOMUT1(METHOD, ARG1T)                 auto& METHOD(ARG1T&& arg1) { return doAutomut(this, &std::remove_reference_t<decltype(*this)>::c##METHOD, std::forward<ARG1T>(arg1)); }
#define AUTOMUT2(METHOD, ARG1T, ARG2T)          auto& METHOD(ARG1T&& arg1, ARG2T&& arg2) { return doAutomut(this, &std::remove_reference_t<decltype(*this)>::c##METHOD, std::forward<ARG1T>(arg1), std::forward<ARG2T>(arg2)); }
#define AUTOMUT3(METHOD, ARG1T, ARG2T, ARG3T)   auto& METHOD(ARG1T&& arg1, ARG2T&& arg2, ARG3T&& arg3) { return doAutomut(this, &std::remove_reference_t<decltype(*this)>::c##METHOD, std::forward<ARG1T>(arg1), std::forward<ARG2T>(arg2), std::forward<ARG3T>(arg3)); }

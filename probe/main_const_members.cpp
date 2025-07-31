#include <utility>
#include <iostream>
#include <string>

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

class S
{
    int m_hidden = 1;
public:
    AUTOMUT0(get_hidden);
    const int& cget_hidden() const
    {
        return m_hidden;
    }

    AUTOMUT1(get_hidden_with_useless_arg, std::string);
    const int& cget_hidden_with_useless_arg(std::string s) const
    {
        return m_hidden;
    }
};

int main()
{
    S s;
    s.get_hidden_with_useless_arg("123") = 3;
    std::cout << s.get_hidden() << "\n";
};

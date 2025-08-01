#pragma once

// Usage: const auto autoop_tied() const { return std::tie(m_...,); }
// TODO: Check if the copy happens
template <typename T>
struct AutoOp
{
    bool operator!=(const T& other) { return !(*this == other); }
    bool operator> (const T& other) { return other < *this; }
    bool operator<=(const T& other) { return !(*this > other); }
    bool operator>=(const T& other) { return !(*this < other); }
};

#define AUTOOP0(T)\
    bool operator!=(const T& other) const { return !(*this == other); }\
    bool operator> (const T& other) const { return   other <  *this ; }\
    bool operator<=(const T& other) const { return !(*this >  other); }\
    bool operator>=(const T& other) const { return !(*this <  other); }
#define AUTOOP1(T, V1)\
    bool operator==(const T& other) const { return V1 == other.V1; }\
    bool operator< (const T& other) const { return V1 <  other.V1; }\
    AUTOOP0(T)
#define AUTOOP2(T, V1, V2)\
    bool operator==(const T& other) const { return std::tie(V1, V2) == std::tie(other.V1, other.V2); }\
    bool operator< (const T& other) const { return std::tie(V1, V2) <  std::tie(other.V1, other.V2); }\
    AUTOOP0(T)
#define AUTOOP3(T, V1, V2, V3)\
    bool operator==(const T& other) const { return std::tie(V1, V2, V3) == std::tie(other.V1, other.V2, other.V3); }\
    bool operator< (const T& other) const { return std::tie(V1, V2, V3) <  std::tie(other.V1, other.V2, other.V3); }\
    AUTOOP0(T)
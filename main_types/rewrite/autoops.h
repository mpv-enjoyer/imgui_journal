#pragma once
#define AUTOOPS0(T)\
    bool operator!=(const T& other) const { return !(*this == other); }\
    bool operator> (const T& other) const { return   other <  *this ; }\
    bool operator<=(const T& other) const { return !(*this >  other); }\
    bool operator>=(const T& other) const { return !(*this <  other); }
#define AUTOOPS1(T, V1)\
    bool operator==(const T& other) const { return V1 == other.V1; }\
    bool operator< (const T& other) const { return V1 <  other.V1; }\
    AUTOOPS0(T)
#define AUTOOPS2(T, V1, V2)\
    bool operator==(const T& other) const { return std::tie(V1, V2) == std::tie(other.V1, other.V2); }\
    bool operator< (const T& other) const { return std::tie(V1, V2) <  std::tie(other.V1, other.V2); }\
    AUTOOPS0(T)
#define AUTOOPS3(T, V1, V2, V3)\
    bool operator==(const T& other) const { return std::tie(V1, V2, V3) == std::tie(other.V1, other.V2, other.V3); }\
    bool operator< (const T& other) const { return std::tie(V1, V2, V3) <  std::tie(other.V1, other.V2, other.V3); }\
    AUTOOPS0(T)

#pragma once
#include <string>

class Teacher
{
    std::string m_name;
    std::string m_abbreviation;
public:
    Teacher(std::string name, std::string abbreviation)
    : m_name(name), m_abbreviation(abbreviation)
    { };
    std::string get_name() const { return m_name; }
    std::string get_abbreviation() const { return m_abbreviation; }
    void set_name(std::string name)
    {
        m_name = name;
    }
    void set_abbreviation(std::string abbreviation)
    {
        m_abbreviation = abbreviation;
    }
};
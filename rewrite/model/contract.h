#pragma once

class Contract
{
    int m_id;
public:
    Contract(int id) : m_id(id) { }
    int get_number() const { return m_id; }
};
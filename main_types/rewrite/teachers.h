#pragma once
#include "lib_vector.h"
#include "teacher.h"

class Teachers
{
    Vector<Teacher> m_teachers;
public:
    Teachers() { }
    const Vector<Teacher>& get() { return m_teachers; }
    Vector<Teacher>& get_mut() { return m_teachers; }
};
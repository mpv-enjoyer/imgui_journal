#pragma once
#include "lib_vector.h"
#include "teacher.h"
#include "automut.h"

class Teachers // Useless?
{
    Vector<Teacher> m_teachers;
public:
    Teachers() { }
    const Vector<Teacher>& cref_data() const { return m_teachers; }
    Vector<Teacher>& ref_data() { return m_teachers; }
};
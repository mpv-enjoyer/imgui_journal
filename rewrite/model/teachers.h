#pragma once
#include "common/vector_sortable.h"
#include "teacher.h"

class Teachers
{
    Vector_Sortable<Teacher> m_teachers;
public:
    Teachers() { }
    const Vector_Sortable<Teacher>& cref_data() const { return m_teachers; }
    Vector_Sortable<Teacher>& ref_data() { return m_teachers; }
};
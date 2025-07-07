#include "all_students.h"

auto AllStudents::get_id_iterator() const -> Dataset::nth_index_const_iterator<0>
{
    return data.get<0>()->const_iterator();
}
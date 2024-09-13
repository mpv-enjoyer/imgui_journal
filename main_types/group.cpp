#include "group.h"

Group::Group(Number number, AgeGroup age_group) : _number(number), _age_group(age_group) { }

const Student& Group::get_student(StudentID id) const
{
    return PTRREF(students[id].student);
}

Group::Number Group::get_number() const
{
    return _number;
}

bool Group::set_number(Number number)
{
    _number = number;
    return true;
}

std::size_t Group::get_size() const
{
    return students.size();
}

Group::AgeGroup Group::get_age_group() const
{
    return _age_group;
}

bool Group::set_age_group(AgeGroup age_group)
{
    if (age_group.value < 0) return false;
    _age_group = age_group;
    return true;
}

bool Group::should_attend(StudentID id, InternalLessonID internal_lesson_id) const
{
    IM_ASSERT(id < students.size());
    Attend_Data attend_data = students[id].attend_data;
    if (is_deleted(id)) return false;
    if (attend_data == ATTEND_FIRST && internal_lesson_id == INTERNAL_LESSON_SECOND) return false;
    if (attend_data == ATTEND_SECOND && internal_lesson_id == INTERNAL_LESSON_FIRST) return false;
    return true; 
}

Group::StudentID Group::find_student(const Student& student) const
{
    for (int i = 0; i < students.size(); i++)
    {
        if (PTRREF(students[i].student) == student) return i;
    }
    return -1;
}

Group::StudentID Group::add_student(Student& new_student)
{
    StudentID id(students.size());
    Students_List list = (Students_List){.student = &new_student, .removed_info = {false, time(NULL)}, .attend_data = ATTEND_BOTH};
    students.push_back(list);
    return id;
};

bool Group::delete_student(StudentID id)
{
    IM_ASSERT(id < students.size());
    if (is_deleted(id)) return false;
    students[id].removed_info.first = true;
    students[id].removed_info.second = time(NULL);
    return true;
}

bool Group::is_in_group(const Student& student, StudentID* id) const
{
    StudentID result = find_student(student);
    if (id != nullptr) *id = result;
    return result.value != -1;
}

bool Group::is_deleted(StudentID id) const
{
    IM_ASSERT(id < students.size());
    return students[id].removed_info.first;
}

bool Group::restore_student(StudentID id)
{
    IM_ASSERT(id < students.size());
    if (!is_deleted(id)) return false;
    students[id].removed_info.first = false;
    students[id].removed_info.second = time(NULL);
    return true;
}

bool Group::set_comment(Comment comment)
{
    _comment = comment;
    return true;
}

Group::Comment Group::get_comment() const
{
    return _comment;
}

std::string Group::get_description() const
{
    std::string output = "#" + std::to_string(get_number().value);
    output.append(", " + Age_Group_Names[get_age_group().value]);
    if (_comment.value.size() != 0) output.append(", " + get_comment().value);
    return output;
}

time_t Group::get_removed_time(StudentID id) const
{
    IM_ASSERT(id < students.size());
    return students[id].removed_info.second;
}

bool Group::operator==(const Group& rhs) const { return this == &rhs; };

Group::AttendData Group::get_attend_data(StudentID id) const
{
    return students[id].attend_data;
}

bool Group::set_attend_data(StudentID id, AttendData attend_data)
{
    IM_ASSERT(attend_data == ATTEND_BOTH || attend_data == ATTEND_FIRST || attend_data == ATTEND_SECOND);
    students[id].attend_data = attend_data;
    return true;
}
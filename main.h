#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <string>
#include <limits>
#include <vector>
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/locid.h>

#include <ctime> //std::tm is used ONLY for YY.MM.DD


struct JTime //used separately with ctime.
{
    unsigned int hours; //0-23
    unsigned int minutes; //0-59
};

inline bool operator==(const JTime& lhs, const JTime& rhs) { return lhs.hours==rhs.hours && lhs.minutes==rhs.minutes; }
inline bool operator!=(const JTime& lhs, const JTime& rhs) { return !(lhs == rhs); }
inline bool operator< (const JTime& lhs, const JTime& rhs) { return lhs.hours < rhs.hours || (lhs.hours == rhs.hours && lhs.minutes < rhs.minutes); }
inline bool operator> (const JTime& lhs, const JTime& rhs) { return rhs < lhs; }
inline bool operator<=(const JTime& lhs, const JTime& rhs) { return !(lhs > rhs); }
inline bool operator>=(const JTime& lhs, const JTime& rhs) { return !(lhs < rhs); }

struct Lesson_Pair
{
    JTime time_begin;
    JTime time_end;
    int lesson_name_id;
};

struct Lesson
{
    int merged_lesson_id;
    int internal_lesson_id;
};

class Student
{
private:
    bool deleted = 0;
    int contract;
    std::string name;
    std::vector<Lesson> lesson_info_ignore_ids; //this breaks a hierarchy, but is kept to allow some students to skip certain lessons.
public:
    Student();
    int get_contract(); bool set_contract(int new_contract);
    std::string get_name(); bool set_name(std::string new_name);
    bool is_ignored(Lesson lesson); bool add_lesson_ignore_id(Lesson new_lesson); bool delete_lesson_ignore_id(Lesson lesson_to_delete); 
    int get_lessons_size();
};

class Group
{
private:
    bool deleted = 0;
    int number;
    std::vector<Student>* all_students;
    std::vector<int> students_sort_by_id;
public:
    Group(std::vector<Student>* students_list);
    std::string comment;
    int get_size();
    int get_number(); bool set_number(int new_number);
    int get_student_sort_id(int student); bool add_student(int student_id); bool delete_student(int student_id);
};

class Lesson_Info //can contain multiple lessons which will be merged in the table.
{
private:
    bool deleted = 0;
    std::vector<Group>* all_groups;
    int group;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Lesson_Info(std::vector<Group>* all_groups);
    int get_group(); bool set_group(int new_group_id);
    Lesson_Pair get_lesson_pair(int id); bool add_lesson_pair(Lesson_Pair new_lesson_pair); bool delete_lesson_pair(int id);
    int get_lessons_size();
};

struct Student_Status
{
    int status;
    std::tm workout_day;
    Lesson workout_lesson;
};

struct Workout_Info
{
    int student_id;
    Lesson lesson;
    std::tm date;
};

struct Archive_Student
{
    Student data;
    std::tm edit_date;
    std::string comment;
};

struct Archive_Group
{
    Group data;
    std::tm edit_date;
    std::string comment;
};

struct Archive_Lesson_Info
{
    Lesson_Info data;
    std::tm edit_date;
    std::string comment;
};

struct Archive
{
    std::vector<Archive_Group> archive_group;
    std::vector<Archive_Student> archive_student;
};

class Calendar_Day
{
private:
    std::vector<Lesson_Info>* lessons;
    std::vector<Group>* all_groups;
    std::vector<Student>* all_students;
    std::vector<std::vector<std::vector<Student_Status>>> student_status; //[merged_lesson][internal_lesson][student_in_group]
    std::vector<std::vector<std::vector<Workout_Info>>> workouts; //[merged_lesson][internal_lesson][new_student]
public:
    Calendar_Day(std::vector<Lesson_Info>* lessons_in_this_day, std::vector<Group>* all_groups, std::vector<Student>* all_students);
    bool set_status(Lesson lesson, int internal_student_id, int status);
    Student_Status get_status(Lesson lesson, int internal_student_id);
    bool add_workout(Lesson lesson, int student_id, std::tm workout_date, Lesson workout_lesson);
    int get_workout_size(Lesson lesson);
    int get_workout_student_id(Lesson lesson, int workout_id);
    bool delete_workout(Lesson lesson, int student_id);

    //the following is needed to properly update the journal

    bool add_student_from_group(int group_id, int student_id);
    bool delete_student_to_group(int group_id, int student_id);
    bool change_group(Lesson lesson, int new_group_id);
    //...
};
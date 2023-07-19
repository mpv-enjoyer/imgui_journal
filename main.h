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

struct JTime //TODO: adaptation for AM PM view
{
    unsigned int hours; //0-23
    unsigned int minutes; //0-59
};

struct Lesson_Pair
{
    JTime time_begin;
    JTime time_end;
    int lesson_name_id;
};

class Student
{
private:
    int id;
    int contract;
    icu::UnicodeString name;
    std::vector<int> lesson_info_ids; //this breaks a hierarchy, but is kept to allow some students to skip certain lessons.
public:
    Student();
    int get_id(); 
    int get_contract(); bool set_contract(int new_contract);
    icu::UnicodeString get_name(); bool set_name(icu::UnicodeString new_name);
    std::vector<int> get_lesson_info_ids(); bool add_lesson_info_id(int new_lesson_info_id); bool delete_lesson_info_id(int lesson_info_id_to_delete); 
};

class Group
{
private:
    int id;
    int number; //visible
    std::vector<Student> students; //sorted by students_id 
    std::vector<int> name_sort_ids; //vector_ids, NOT STUDENTS
public:
    Group();
    icu::UnicodeString comment;
    int get_id(); 
    int get_number(); bool set_number(int new_number);
    std::vector<Student> get_students(); bool add_student(Student new_student); bool delete_student(int to_remove_student); //sorted by name ascending
};

class Lesson_Info //can contain multiple lessons which will be merged in the table.
{
private:
    int id;
    Group group;
    int day_of_the_week; //0-6 (Currently starting from Monday. TODO: add configuration for other calendar layout)
    std::vector<Lesson_Pair> lesson_pairs;
public:
    int get_id();
    Group get_group(); bool set_group(Group new_group);
    int get_day_of_the_week(); bool set_day_of_the_week(int new_day);
    Lesson_Pair get_lesson_pair(); bool set_lesson_pair(Lesson_Pair new_lesson_pair);
};

static int student_max_id = -1;
static int group_max_id = -1;
static int lesson_info_max_id = -1;
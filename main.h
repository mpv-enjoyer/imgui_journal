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

class Student
{
private:
    int id;
    int contract;
    std::string name;
public:
    int get_id(); 
    std::u16string hh;
    int get_contract(); std::string get_name();
    bool set_contract(int new_contract); bool set_name(std::string new_name);
    Student();
    ~Student();
};

class Group
{
private:
    int id;
    int number; //visible
    std::vector<Student> students; //sorted by students_id 
    std::vector<int> name_sort_ids; //vector_ids, NOT STUDENTS
public:
    int get_id(); 
    int get_number(); bool set_number(int number);
    std::vector<Student> get_students(); //sorted by name descending
    bool add_student(Student new_student);
    bool delete_student(int to_remove_student); //sorted by name descending
    Group();
    ~Group();
};

static int student_max_id = -1;
static int group_max_id = -1;
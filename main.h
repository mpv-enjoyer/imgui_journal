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
#include "imgui/misc/cpp/imgui_stdlib.h"

#include <ctime> //std::tm is used ONLY for YY.MM.DD

#define STATUS_INVALID     -2
#define STATUS_NOT_AWAITED -1
#define STATUS_NO_DATA      0
#define STATUS_ON_LESSON    1
#define STATUS_WAS_ILL      2
#define STATUS_SKIPPED      3
#define STATUS_WORKED_OUT   4

#define MDAY_DIFF           1

#define DEFAULT_COLUMN_COUNT 5
#define AGE_GROUP_COUNT      8
#define MAX_MERGED_LESSONS   2

struct JTime //used separately with ctime.
{
    int hours; //0-23
    int minutes; //0-59
};

inline bool operator==(const JTime& lhs, const JTime& rhs) { return lhs.hours==rhs.hours && lhs.minutes==rhs.minutes; }
inline bool operator!=(const JTime& lhs, const JTime& rhs) { return !(lhs == rhs); }
inline bool operator< (const JTime& lhs, const JTime& rhs) { return lhs.hours < rhs.hours || (lhs.hours == rhs.hours && lhs.minutes < rhs.minutes); }
inline bool operator> (const JTime& lhs, const JTime& rhs) { return rhs < lhs; }
inline bool operator<=(const JTime& lhs, const JTime& rhs) { return !(lhs > rhs); }
inline bool operator>=(const JTime& lhs, const JTime& rhs) { return !(lhs < rhs); }

const  std::string Lesson_Names[] = {"ИЗО", "Лепка", "Дизайн", "Черчение", "Спецкурс"};
const  int         Lesson_Prices[4][3] = {{100, 99, 98}, {200, 199, 198}, {300, 299, 298}, {400, 399, 398}};
const  std::string Month_Names[] = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
const  std::string Day_Names[] = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
const  std::string Age_Group_Names[] = {"4 года, дошкольная группа", "5 лет, дошкольная группа", "6 лет, дошкольная группа", "7 лет, школьная группа", "8 лет, школьная группа", "9 лет, школьная группа", "10-11 лет, школьная группа", "12-13 лет, школьная группа"};

static void HelpMarker(const char* desc) //TODO: possible problems with different fonts.
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

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

struct Lesson_Full
{
    Lesson lesson;
    int day_of_the_week;
};

inline bool operator==(const Lesson& lhs, const Lesson& rhs) { return lhs.internal_lesson_id == rhs.internal_lesson_id && lhs.merged_lesson_id == rhs.merged_lesson_id; }
inline bool operator!=(const Lesson& lhs, const Lesson& rhs) { return !(lhs==rhs);}

class Student
{
private:
    bool removed = 0;
    int contract;
    std::string name;
    int age_group = -1;
    std::vector<Lesson_Full> lessons_ignore; //this breaks a hierarchy, but is kept to allow some students to skip certain lessons.
public:
    Student();
    int get_contract(); bool set_contract(int new_contract);
    std::string get_name(); bool set_name(std::string new_name);
    std::string get_age_group(); bool set_age_group(int new_age_group);
    bool is_ignored(Lesson lesson, int lesson_day_of_the_week); bool add_lesson_ignore_id(Lesson new_lesson, int new_lesson_day_of_the_week); bool delete_lesson_ignore(Lesson lesson_to_delete, int day_of_the_week); 
    int get_lessons_size();
    bool is_removed(); bool remove(); bool restore();
};

class Group
{
private:
    bool removed = 0;
    int number;
    std::vector<Student>* all_students;
    std::vector<int> students_sort_by_id;
    std::string comment;
public:
    Group(std::vector<Student>* students_list);
    int get_size();
    int get_number(); bool set_number(int new_number);
    int get_student_sort_id(int student); int add_student(int student_id); bool delete_student(int student_id);
    std::string get_comment(); bool set_comment(std::string new_comment);
    std::string get_description();
    bool is_in_group(int student);
};

class Lesson_Info //can contain multiple lessons which will be merged in the table.
{
private:
    bool removed = 0;
    std::vector<Group>* all_groups;
    int group;
    std::vector<Lesson_Pair> lesson_pairs;
public:
    Lesson_Info(std::vector<Group>* all_groups);
    int get_group(); bool set_group(int new_group_id);
    Lesson_Pair get_lesson_pair(int id); bool add_lesson_pair(Lesson_Pair new_lesson_pair); bool delete_lesson_pair(int id);
    bool remove();
    int get_lessons_size();
    std::string get_description();
};

struct Student_Status
{
    int student_id; //DO NOT USE
    int status;
    std::tm workout_day;
    Lesson workout_lesson;
    int discount_status;
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
    Calendar_Day(std::vector<Lesson_Info>* lessons_in_this_day, std::vector<Group>* all_groups, std::vector<Student>* all_students, int current_day_of_the_week);
    bool set_status(Lesson lesson, int student_id, int status);
    Student_Status get_status(Lesson lesson, int student_id);
    bool add_workout(Lesson lesson, int student_id, std::tm workout_date, Lesson workout_lesson);
    int get_workout_size(Lesson lesson);
    int get_workout_student_id(Lesson lesson, int workout_id);
    bool delete_workout(Lesson lesson, int student_id);
    bool set_discount_status(Lesson lesson, int student_id, int discount_status);
    int get_discount_status(Lesson lesson, int student_id);

    //the following is needed to properly update the journal

    bool add_student_to_group(int group_id, int student_id, int new_student_id);
    bool delete_student_from_group(int group_id, int student_id); //not needed?
    bool change_group(Lesson lesson, int new_group_id);
    bool change_lesson_pair(Lesson lesson, Lesson_Pair new_lesson_pair);
    bool add_merged_lesson(int day_of_the_week, Lesson_Info new_lesson_info, bool await_no_one, int merged_lesson_id);
    //...
};

//Popups & secondary windows
static ImGuiTextFilter popup_add_student_to_group_filter;
bool popup_add_student_to_group(std::vector<Student>* all_students, std::vector<Group>* all_groups, std::vector<Calendar_Day>* all_days, int current_group_id, int* selected_to_add);
bool popup_select_day_of_the_week(int* selected_day_of_the_week, int* selected_month);
bool students_list(std::vector<Student>* all_students, std::vector<Group>* all_groups);
bool popup_add_student_to_base(Student* new_student, bool* ignore, bool erase_input);
bool popup_add_merged_lesson_to_journal(std::vector<Group>* all_groups, Lesson_Info* new_lesson_info, int current_day_of_the_week, bool* ignore, bool erase_input);
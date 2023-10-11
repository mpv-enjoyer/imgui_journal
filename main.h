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

#include "imgui/misc/cpp/imgui_stdlib.h"

#include <ctime> //std::tm is used ONLY for YY.MM.DD

#define STATUS_INVALID     -2
#define STATUS_NOT_AWAITED -1
#define STATUS_NO_DATA      0
#define STATUS_ON_LESSON    1
#define STATUS_WAS_ILL      2
#define STATUS_SKIPPED      3
#define STATUS_WORKED_OUT   4

#define MDAY_DIFF            1
#define DEFAULT_COLUMN_COUNT 5
#define AGE_GROUP_COUNT      8
#define MAX_INTERNAL_LESSONS 2
#define SUBCOLUMN_WIDTH_PXLS 30

#define ONE_LINE(STD_STRINGS)        one_line(STD_STRINGS).c_str()
#define POPUP_INIT_FRAME(POPUP_NAME) ImGui::OpenPopup(POPUP_NAME); ImVec2 center = ImGui::GetMainViewport()->GetCenter(); ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f)); if (ImGui::BeginPopupModal(POPUP_NAME, NULL, ImGuiWindowFlags_AlwaysAutoResize))
#define POPUP_OK                     { ok(); ImGui::CloseCurrentPopup(); ImGui::EndPopup(); return true; }
#define POPUP_CANCEL                 { cancel(); ImGui::CloseCurrentPopup(); ImGui::EndPopup(); return true; }

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

const std::vector<std::string> Lesson_Names = {"ИЗО", "Лепка", "Дизайн", "Черчение", "Спецкурс"};
const char                     Lesson_Names_Combo[] = "ИЗО\0Лепка\0Дизайн\0Черчение\0Спецкурс\0\0";
const int                      Lesson_Prices[5][3] = {{100, 99, 98}, {200, 199, 198}, {300, 299, 298}, {400, 399, 398}, {500, 499, 498}};
const std::vector<std::string> Month_Names = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
const char                     Month_Names_Combo[] = "Январь\0Февраль\0Март\0Апрель\0Май\0Июнь\0Июль\0Август\0Сентябрь\0Октябрь\0Ноябрь\0Декабрь\0\0";
const std::vector<std::string> Day_Names = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
const char                     Day_Names_Combo[] = "Воскресенье\0Понедельник\0Вторник\0Среда\0Четверг\0Пятница\0Суббот\0\0";
const std::vector<std::string> Day_Names_Abbreviated = {"Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};
const char                     Day_Names_Abbreviated_Combo[] = "Вс\0Пн\0Вт\0Ср\0Чт\0Пт\0Сб\n\n";
const std::vector<std::string> Age_Group_Names = {"4 года, дошкольная группа", "5 лет, дошкольная группа", "6 лет, дошкольная группа", "7 лет, школьная группа", "8 лет, школьная группа", "9 лет, школьная группа", "10-11 лет, школьная группа", "12-13 лет, школьная группа"};
const char                     Age_Group_Names_Combo[] = " 4 года, дошкольная группа\0 5 лет, дошкольная группа\0 6 лет, дошкольная группа\0 7 лет, школьная группа\0 8 лет, школьная группа\0 9 лет, школьная группа\0 10-11 лет, школьная группа\0 12-13 лет, школьная группа\0\0";

static void HelpMarker(const char* desc)
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

struct Group_Pair
{
    int day_of_the_week;
    int number;
};

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
    int get_age_group(); std::string get_age_group_string(); bool set_age_group(int new_age_group);
    bool is_ignored(Lesson lesson, int lesson_day_of_the_week); bool add_lesson_ignore_id(Lesson new_lesson, int new_lesson_day_of_the_week); bool delete_lesson_ignore(Lesson lesson_to_delete, int day_of_the_week); 
    int get_lessons_size();
    bool is_removed(); bool remove(); bool restore();
};

class Group
{
private:
    bool removed = 0;
    Group_Pair group_info;
    std::vector<Student>* all_students;
    std::vector<int> students_sort_by_id;
    std::string comment;
    std::vector<int> deleted_students_sort_by_id;
public:
    Group(std::vector<Student>* students_list);
    int get_size();
    int get_number(); bool set_number(int new_number);
    int get_day_of_the_week();
    bool set_day_of_the_week(int new_day);
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
    bool should_attend(int student);
    int get_lessons_size();
    std::string get_description(int current_internal_lesson = -1);
};

struct Student_Status
{
    int student_id; //DO NOT USE
    int status;
    std::tm workout_day;
    Lesson workout_lesson;
    int discount_status = -1;
};

struct Workout_Info
{
    int student_id;
    Lesson_Pair lesson_pair;
    std::tm date; //day
};

class Calendar_Day
{
private:
    std::vector<Lesson_Info>* lessons;
    std::vector<Group>* all_groups;
    std::vector<Student>* all_students;
    std::vector<std::vector<std::vector<Student_Status>>> student_status; //[merged_lesson][internal_lesson][student_in_group]
    //TODO: change student_status structure to ID-based? (for stability)
    std::vector<std::vector<std::vector<Workout_Info>>> workouts; //[merged_lesson][internal_lesson][new_student]
public:
    Calendar_Day(std::vector<Lesson_Info>* lessons_in_this_day, std::vector<Group>* all_groups, std::vector<Student>* all_students, int current_day_of_the_week);
    bool set_status(Lesson lesson, int student_id, int status);
    Student_Status get_status(Lesson lesson, int student_id);
    bool add_workout(Lesson lesson, int student_id, std::tm workout_date, Lesson_Pair workout_lesson);
    bool add_workout(Lesson lesson, Workout_Info new_workout_info);
    int get_workout_size(Lesson lesson);
    int get_workout_student_id(Lesson lesson, int workout_id);
    Workout_Info get_workout_info(Lesson lesson, int student_id);
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
static ImGuiTextFilter popup_add_working_out_filter;
static int popup_edit_ignore_lessons_is_open = -1;
static int popup_add_working_out_select_day = -1;
static int popup_add_working_out_select_month = -1;
static int popup_add_working_out_select_year = -1;
static Lesson popup_add_working_out_select_lesson = {0,0};

bool students_list(std::vector<Student>* all_students, std::vector<Group>* all_groups, int* popup_edit_ignore_lessons_is_open);
bool popup_add_merged_lesson_to_journal(std::vector<Group>* all_groups, Lesson_Info* new_lesson_info, Group* new_group, int current_day_of_the_week, bool* ignore, bool erase_input);
bool popup_edit_ignore_lessons(std::vector<std::vector<Lesson_Info>>* lessons_in_a_week, std::vector<Student>* all_students, int current_student_id, bool* ignore);
bool popup_add_working_out(std::vector<Student>* all_students, std::vector<Group>* all_groups, std::vector<Calendar_Day>* all_days, std::vector<std::vector<Lesson_Info>>* all_lessons, int current_group_id, int* selected_to_add, int first_mwday, int number_of_days, Workout_Info* lesson_to_workout);

class Popup
{
private:
    bool accept_edit = false;
public:
    Popup() { };
    bool check_ok() { return accept_edit; }
    bool cancel() { accept_edit = false; return true; }
    bool ok() { accept_edit = true; return true; }
};

class Popup_Add_Student_To_Group : public Popup
{
private:
    int current_group_id = -1;
    int merged_lesson = -1;
    int current_selected_student = -1;
    ImGuiTextFilter text_filter;
    std::vector<Student>* all_students = nullptr;
    std::vector<Group>* all_groups = nullptr;
    std::vector<std::string> possible_student_descriptions;
    std::vector<int> possible_student_ids;
public:
    Popup_Add_Student_To_Group()
    {        
        for (int i = 0; i < all_students->size(); i++)
        {
            if (all_students->at(i).is_removed()) continue;
            if (all_groups->at(current_group_id).is_in_group(i)) continue;
            possible_student_descriptions.push_back((all_students->at(i).get_name() + " (" + std::to_string(all_students->at(i).get_contract()) + ")"));
            possible_student_ids.push_back(i);
        }
    };
    Popup_Add_Student_To_Group(int current_group_id, std::vector<Student>* all_students, std::vector<Group>* all_groups, int merged_lesson) 
    : current_group_id(current_group_id), all_students(all_students), all_groups(all_groups), merged_lesson(merged_lesson)
    {
        IM_ASSERT(!(current_group_id >= all_groups->size() || current_group_id < 0 || merged_lesson < 0));
        for (int i = 0; i < all_students->size(); i++)
        {
            if (all_students->at(i).is_removed()) continue;
            if (all_groups->at(current_group_id).is_in_group(i)) continue;
            possible_student_descriptions.push_back((all_students->at(i).get_name() + " (" + std::to_string(all_students->at(i).get_contract()) + ")"));
            possible_student_ids.push_back(i);
        }
    };
    int get_merged_lesson() { IM_ASSERT(check_ok()); return merged_lesson; };
    int get_added_student() { IM_ASSERT(check_ok()); return current_selected_student; };
    int get_current_group_id() { IM_ASSERT(check_ok()); return current_group_id; };
    bool show_frame();
    bool is_ok_possible(bool select_visible) { return select_visible && current_selected_student!=-1; }
};

class Popup_Select_Day_Of_The_Week : public Popup
{
private:
    int day_of_the_week = 0;
    int month = 0;
    int year = 0;
public:
    Popup_Select_Day_Of_The_Week() {};
    Popup_Select_Day_Of_The_Week(int current_day_of_the_week, int current_month, int current_year)
    {
        day_of_the_week = current_day_of_the_week;
        month = current_month;
        year = current_year + 1900;
    }
    bool show_frame();
    bool is_ok_possible() { return true; }
    int get_day_of_the_week() { IM_ASSERT(check_ok()); return day_of_the_week; }
    int get_month() { IM_ASSERT(check_ok()); return month; }
    int get_year() { IM_ASSERT(check_ok()); return year - 1900; }
};

class Popup_Add_Student_To_Base : public Popup
{
private:
    std::string name = "";
    bool is_date_visible = false;
    int contract = 0;
    int age_group = 0;
public:
    Popup_Add_Student_To_Base() {};
    bool show_frame();
    Student get_new_student()
    {
        IM_ASSERT(check_ok());
        Student output;
        output.set_name(name);
        output.set_contract(contract);
        if (is_date_visible) output.set_age_group(age_group);
        return output;
    }
    bool is_ok_possible() { return contract >= 0; }
};

class Popup_Add_Merged_Lesson_To_Journal : public Popup
{
private:
    int group_number = 0;
    std::string group_comment;
    int group_id = -1;
    int combo_lesson_name_id = 0;
    Lesson_Pair new_lesson_pairs[];
    //TODO:
};

//Date & time things
int calculate_first_mwday(int current_mday, int current_wday);
int get_first_wday(int month, int year, int wday);
int get_number_of_days(int month, int year);

//to replace with actual algorithms
template <typename T = int>
bool is_in_vector(std::vector<T> vector, T to_find)
{
    for (int i = 0; i < vector.size(); i++)
    {
        if (to_find == vector[i]) return true;
    }
    return false;
};

const char* c_str_int(int num);
std::string to_string(JTime value);
std::string to_string(JTime begin, JTime end);
std::string to_string(std::tm day, JTime begin, JTime end = {-1, -1}, bool abbreviate = true);
std::string generate_label(const std::string prefix, std::vector<int> unique);
bool j_button_selectable(const char* label, bool selected);
bool j_input_time(std::string label, JTime& time);
bool j_attendance_combo(const char* label, int* status);

/*
Currently used labels:

 ##combo_attendance
 O##add_workout_button
 ##workout_info_radio_tooltip
 ##fake_radio
 Добавить ученика##
 ##dummy1
 ##dummy2
 Day_Names[i]##change_day_button

*/
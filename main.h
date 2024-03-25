#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <string>
#include <limits>
#include <vector>
#include <array>
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif
#include "imgui/misc/cpp/imgui_stdlib.h"
#include <ctime> //std::tm is used ONLY for YY.MM.DD

#include <fstream>
#include <sstream>
#include <algorithm>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/assume_abstract.hpp>

#define STATUS_INVALID     -2
#define STATUS_NOT_AWAITED -1
#define STATUS_NO_DATA      0
#define STATUS_ON_LESSON    1
#define STATUS_WAS_ILL      2
#define STATUS_SKIPPED      3
#define STATUS_WORKED_OUT   4

//all date related functions return 1-based day so
//we use -MDAY_DIFF to go back to 0-based day.
#define MDAY_DIFF            1

#define DEFAULT_COLUMN_COUNT 5
#define MAX_INTERNAL_LESSONS 2
#define SUBCOLUMN_WIDTH_PXLS 40
#define CONVERT_TO_RU_CALENDAR(wday) wday == 6 ? 0 : wday + 1;

#define ONE_LINE(STD_STRINGS)        one_line(STD_STRINGS).c_str()
#define POPUP_INIT_FRAME(POPUP_NAME) ImGui::OpenPopup(POPUP_NAME); \
                                     ImVec2 center = ImGui::GetMainViewport()->GetCenter(); \
                                     ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f)); \
                                     if (ImGui::BeginPopupModal(POPUP_NAME, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
#define POPUP_OK                     { ok(); ImGui::CloseCurrentPopup(); ImGui::EndPopup(); return true; }
#define POPUP_CANCEL                 { cancel(); ImGui::CloseCurrentPopup(); ImGui::EndPopup(); return true; }
#define PTRREF(PTR)                  *(PTR)
#define WINDOW_FLAGS                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove

typedef short Attend_Data;
#define ATTEND_BOTH 0
#define ATTEND_FIRST 1
#define ATTEND_SECOND 2
#define INTERVAL_CHECK(left1, right1, left2, right2) (!(left1 < right1 && right1 < left2 && left2 < right2))

const int LESSON_TYPE_COUNT = 5;
const int LESSON_PRICES_COUNT = 3;
const int AGE_GROUP_COUNT = 8;

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

class Student;
class Group;
class Lesson_Info;
class Calendar_Day;
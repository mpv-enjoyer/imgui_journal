#pragma once
#include "../storage/journal.h"

class Mainwindow;

/*
    JournalHolder (previously Graphical) is:
    1) (V) Journal storage.
    2) (X) Subwindow handler
    3) Some info about user inputs: wday and edit_mode
    4) (X) additional ImGui functions
*/

namespace Graphical
{
    bool attend_data(std::string label, Attend_Data *attend_data, std::string first_lesson_name, std::string second_lesson_name);
    bool button_selectable(const char *label, bool selected, bool small = false);
    bool button_dangerous(const char *label);
    bool input_time(std::string label, JTime &time);
    bool button_colored(const char *label, float r, float g, float b);
    bool age_group_combo(const char *label, int *age_group, bool shrink = true);
    bool attendance_combo(const char* label, int* status, std::string tooltip = "");
    // RAII versions of ImGui functions:
    struct Group
    {
        [[nodiscard]] Group() { ImGui::BeginGroup(); }
        ~Group() { ImGui::EndGroup(); }
    };
    struct Child
    {
        const bool valid;
        [[nodiscard]] Child(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags window_flags = 0)
        : valid(ImGui::BeginChild(str_id, size, border, window_flags)) { }
        ~Child() { ImGui::EndChild(); } // "Always call a matching EndChild() for each BeginChild() call" from docs
    };
    class StudentPicker
    {
        ImGuiTextFilter filter;
        int current = -1;
        std::vector<std::string> _descriptions;
        std::vector<int> _id_list;
        bool use_id_list = false;
    public:
        [[nodiscard]] StudentPicker() {};
        [[nodiscard]] StudentPicker(std::vector<std::string> descriptions, std::vector<int> id_list = {});
        [[nodiscard]] int show();
        [[nodiscard]] bool is_valid();
    };
};

class JournalHolder /* move this class to journal_holder.h? */
{
    bool _edit_mode;
    int _wday;
    std::vector<Day_With_Info> _visible_days;
public:
//    Mainwindow* mainwindow = nullptr;
    Journal& journal;

    JournalHolder(Journal& main_journal);
    const int& wday = std::ref(_wday);
    const std::vector<Day_With_Info>& visible_days = std::ref(_visible_days);
    const bool& edit_mode = std::ref(_edit_mode);
    void select_wday(int wday);
    void set_edit_mode(bool value);
};
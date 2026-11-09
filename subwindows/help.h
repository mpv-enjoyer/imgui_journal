#pragma once
#include "subwindows.h"

void show_button_to_recalculate_all_prices();
void show_button_to_open_workout_debugging();

class Subwindow_Help : public Subwindow
{
    const ImVec4 background = ImVec4(225.f/255.f, 240.f/255.f, 253.f/255.f, 1.0f);
    struct Image
    {
        std::string name;
        int width;
        int height;
        GLuint texture;
        bool loaded = false;
        Image(std::string name);
    };
    Image add_student_to_base = Image("add_student_to_base.png");
    Image add_group = Image("add_group.png");
    Image add_student_to_group = Image("add_student_to_group.png");
    Image info = Image("exclamation.png");
    Image workout1 = Image("workout1.png");
    Image workout2 = Image("workout2.png");
    Image workout3 = Image("workout3.png");
    Image workout4 = Image("workout4.png");
    Image attendance = Image("attendance.png");
    Image students_list = Image("students_list.png");
    Image groups_list = Image("groups_list.png");
    Image edit_attend_data = Image("edit_attend_data.png");
    Image student_search_1 = Image("student_search_1.png");
    Image student_search_2 = Image("student_search_2.png");
    Image move_to_group_1 = Image("move_to_group_1.png");
    Image move_to_group_2 = Image("move_to_group_2.png");
    Image move_to_group_3 = Image("move_to_group_3.png");
    Image move_to_group_4 = Image("move_to_group_4.png");
public:
    Subwindow_Help(JournalHolder *graphical, Popup_Handler* popup_handler);
    bool draw_image(Image image);
    void draw_note(std::string text);
    bool show_frame() override;
    bool virtual allow_ontop() override { return false; };
};
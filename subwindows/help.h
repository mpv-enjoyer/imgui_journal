#pragma once
#include "subwindows.h"

class Subwindow_Help : public Subwindow
{
    struct Image
    {
        std::string name;
        int width;
        int height;
        GLuint texture;
        bool loaded;
        Image(std::string name);
    };
    const ImVec4 background = ImVec4(0.85f, 0.85f, 0.7f, 1.0f);
    Image add_student_to_base = Image("add_student_to_base.png");
    Image add_group = Image("add_group.png");
    Image add_student_to_group = Image("add_student_to_group.png");
    Image info = Image("exclamation.png");
    Image workout1 = Image("workout1.png");
    Image workout2 = Image("workout2.png");
    Image workout3 = Image("workout3.png");
    Image workout4 = Image("workout4.png");
public:
    Subwindow_Help(Graphical *_graphical);
    bool draw_image(Image image);
    void draw_note(std::string text);
    bool show_frame();
};
#include "help.h"

Subwindow_Help::Subwindow_Help(Graphical *_graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
}

bool draw_image(std::string name)
{
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    bool ret = LoadTextureFromFile(("images/" + name).c_str(), &my_image_texture, &my_image_width, &my_image_height);
    if (!ret)
    {
        ImGui::TextDisabled(name.c_str());
        return false;
    }
    ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
    return true;
}

bool Subwindow_Help::show_frame()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("Справка", nullptr, WINDOW_FLAGS);

    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }
    ImGui::BeginChild("Child", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    draw_image("example.png");
    draw_image("example2.png");

    ImGui::EndChild();
    ImGui::End();
    return false;
}

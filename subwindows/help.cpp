#include "help.h"

Subwindow_Help::Subwindow_Help(Graphical *_graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
}

bool Subwindow_Help::draw_image(Image image)
{
    Image current = image;
    if (!(current.loaded))
    {
        ImGui::TextColored(ImVec4(0.8, 0.2, 0.2, 1), "not found: %s", image.name.c_str());
        return false;
    }
    ImGui::Image((void*)(intptr_t)(current.texture), ImVec2(current.width, current.height));
    return true;
}

void Subwindow_Help::draw_note(std::string text)
{
    const ImVec4 background = ImVec4(255.f/115.f, 255.f/140.f, 255.f/153.f, 1.0f);
    draw_image(info);
    ImGui::SameLine();
    ImGui::TextWrapped(text.c_str());
}

void draw_text(std::string string)
{
    ImGui::TextWrapped(string.c_str());
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

    if (ImGui::CollapsingHeader("Как добавить ученика в базу?"))
    {
        draw_text("Для того, чтобы добавлять ученика в группу и отмечать его в журнале, надо сначала добавить его в общую базу.");
        draw_text("1. Перейдите во вкладку 'Ученики'");
        draw_text("2. Нажмите на кнопку 'Добавить ученика'");
        draw_text("3. Введите номер договора и ФИ ученика");
        draw_text("4. Нажмите ОК");
        draw_image(add_student_to_base);
        draw_note("Номер договора и ФИ ученика можно будет изменить потом.");
    }
    if (ImGui::CollapsingHeader("Как добавить группу?"))
    {
        draw_text("1. Перейдите во вкладку 'Группы'");
        draw_text("2. Нажмите на кнопку 'Добавить группу'");
        draw_text("3. Укажите день недели, программу, время, номер группы и возраст");
        draw_text("4. Нажмите ОК");
        draw_image(add_group);
        draw_note("Введённая информация о группе будет видна в описании. Используйте поле 'Описание' только для дополнительных заметок.");
        draw_note("Всю введенную информацию кроме дня недели и программы будет изменить потом.");
    }
    if (ImGui::CollapsingHeader("Как добавить ученика в группу?"))
    {
        draw_text("1. Найдите нужную группу в общей таблице и нажмите на кнопку 'Добавить ученика'");
        draw_text("2. Выберите ученика или нескольких учеников из списка");
        draw_text("3. Нажмите ОК");
        draw_image(add_student_to_group);
        draw_note("Если кнопка 'Добавить ученика' не нажимается, в базе нет подходящих учеников.");
    }
    if (ImGui::CollapsingHeader("Как добавить отработку?"))
    {
        draw_text("1. Найдите группу, в которую ученик фактически пришёл и нажмите на кнопку 'Отр' под нужной датой");
        draw_text("2. Выберите ученика в списке");
        draw_text("3. Выберите день, в который ученик должен прийти");
        draw_text("4. Выберите группу, в которую ученик должен прийти");
        draw_text("5. Нажмите ОК");
    }
    if (ImGui::CollapsingHeader("Пример добавления отработки"))
    {
        draw_text("Иван Иванов должен прийти в группу #1 4 сентября на ИЗО, но пришёл в группу #2 11 сентября");
        draw_image(workout1);
        draw_image(workout2);
        draw_image(workout3);
        draw_image(workout4);
    }
    ImGui::EndChild();
    ImGui::End();
    return false;
}

Subwindow_Help::Image::Image(std::string name) : name(name)
{
    loaded = LoadTextureFromFile(("images/" + name).c_str(), &texture, &width, &height);
}

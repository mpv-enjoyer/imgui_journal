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
    draw_image(info);
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
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
    ImGui::PushStyleColor(ImGuiCol_ChildBg, background);

    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::PopStyleColor();
        ImGui::End();
        return true;
    }
    ImGui::Text("Справка");
    ImGui::BeginChild("Child", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    if (ImGui::CollapsingHeader("Основная информация", ImGuiTreeNodeFlags_DefaultOpen))
    {
        draw_text("Программа состоит из 3 разделов, не считая справки и вкладки изменения цен: ");
        draw_text("1. Журнал. В этом разделе можно отмечать учеников и учителей, добавлять отработки и добавлять учеников в группы:");
        draw_image(attendance);
        draw_text("2. Список учеников. В этом разделе можно изменить информацию о любом из учеников и можно добавить нового ученика в базу:");
        draw_image(students_list);
        draw_text("3. Список групп. В этом разделе можно изменить информацию о всех группах и можно добавить новую группу:");
        draw_image(groups_list);
        draw_text("");
        draw_text("В нижней части журнала находятся кнопки для переключения между днями недели и месяцами.");
        draw_note("Почти всю информацию (напр. ФИ учеников, номера групп) можно редактировать только для текущего месяца.");
        draw_note("Отмечать учеников и добавлять отработки можно и для предыдущих месяцев.");
    }
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
        draw_note("Всю введенную информацию кроме дня недели и программы можно будет изменить потом.");
    }
    if (ImGui::CollapsingHeader("Как добавить ученика в группу?"))
    {
        draw_note("Перед этим надо добавить ученика в общую базу данных (см. Как добавить ученика в базу?)");
        draw_text("1. Найдите нужную группу в общей таблице и нажмите на кнопку 'Добавить ученика'");
        draw_text("2. Выберите ученика или нескольких учеников из списка");
        draw_text("3. Нажмите ОК");
        draw_image(add_student_to_group);
        draw_note("Если кнопка 'Добавить ученика' не нажимается, в базе нет подходящих учеников.");
    }
    if (ImGui::CollapsingHeader("Какие группы отображаются справа от других?"))
    {
        draw_text("Если у групп полностью совпадает время начала первого занятия, то в журнале одна группа показывается правее другой");
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
    if (ImGui::CollapsingHeader("Как добавить ученика, который ходит в одной группе на ИЗО, а в другой на лепку?"))
    {
        draw_text("1. Добавьте ученика в обе группы");
        draw_text("2. Перейдите во вкладку 'Ученики'");
        draw_text("3. Для соответствующего ученика и соответствующих групп выберите ИЗО или лепку");
        draw_image(edit_attend_data);
        draw_note("Ученик может посещать ИЗО + ИЗО, Лепка + Лепка + Дизайн и любые другие комбинации");
    }
    //if (ImGui::CollapsingHeader("Как считаются цены?"))
    //{
    //    draw_text("В журнале рядом с ФИ ученика и его номером договора написана цена.");
    //    draw_text("Эта цена вычисляется с учётом программы группы, количества учеников с таким же номером договора и количеством занятий, на которые ходит этот ученик.");
    //    draw_note("Если после выставления статуса посещения ученика цена меняется, она не меняется в поле статуса");
    //}
    draw_text("");
    draw_text("BUILD " + std::string(__DATE__) + " " + std::string(__TIME__));
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();
    return false;
}

Subwindow_Help::Image::Image(std::string name) : name(name)
{
    loaded = LoadTextureFromFile(("images/" + name).c_str(), &texture, &width, &height);
}

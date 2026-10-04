#include "help.h"
#include "../platforms/platforms.h"
#include "../popups/workout_debugging.h"

bool _button_recalculate_all_prices = false;
bool _button_open_workout_debugging = false;

struct Images
{
    struct Image
    {
        std::string name;
        int width;
        int height;
        GLuint texture;
        bool loaded = false;
        Image(std::string name)
        {
            if (!Impl::renderer()->supports_images())
            {
                loaded = false;
            }
            else
            {
                loaded = LoadTextureFromFile(("images/" + name).c_str(), &texture, &width, &height);
            }
        }
        bool draw() const
        {
            if (!loaded)
            {
                ImGui::TextColored(ImVec4(0.8, 0.2, 0.2, 1), "not found/images not supported: %s", name.c_str());
                return false;
            }
            ImGui::Image((void*)(intptr_t)(texture), ImVec2(width, height));
            return true;
        }
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
};

static const Images& get_images()
{
    static Images images = Images();
    return images;
}

Subwindow_Help::Subwindow_Help(JournalHolder *graphical, Popup_Handler* popup_handler)
: Subwindow(graphical, popup_handler) { }

void Subwindow_Help::draw_note(std::string text)
{
    get_images().info.draw();
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
        get_images().attendance.draw();
        draw_text("2. Список учеников. В этом разделе можно изменить информацию о любом из учеников и можно добавить нового ученика в базу:");
        get_images().students_list.draw();
        draw_text("3. Список групп. В этом разделе можно изменить информацию о всех группах и можно добавить новую группу:");
        get_images().groups_list.draw();
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
        get_images().add_student_to_base.draw();
        draw_note("Номер договора и ФИ ученика можно будет изменить потом.");
    }
    if (ImGui::CollapsingHeader("Как добавить группу?"))
    {
        draw_text("1. Перейдите во вкладку 'Группы'");
        draw_text("2. Нажмите на кнопку 'Добавить группу'");
        draw_text("3. Укажите день недели, программу, время, номер группы и возраст");
        draw_text("4. Нажмите ОК");
        get_images().add_group.draw();
        draw_note("Введённая информация о группе будет видна в описании. Используйте поле 'Описание' только для дополнительных заметок.");
        draw_note("Всю введенную информацию кроме дня недели и программы можно будет изменить потом.");
    }
    if (ImGui::CollapsingHeader("Как добавить ученика в группу?"))
    {
        draw_note("Перед этим надо добавить ученика в общую базу данных (см. Как добавить ученика в базу?)");
        draw_text("1. Найдите нужную группу в общей таблице и нажмите на кнопку 'Добавить ученика'");
        draw_text("2. Выберите ученика или нескольких учеников из списка");
        draw_text("3. Нажмите ОК");
        get_images().add_student_to_group.draw();
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
        get_images().workout1.draw();
        get_images().workout2.draw();
        get_images().workout3.draw();
        get_images().workout4.draw();
    }
    if (ImGui::CollapsingHeader("Как добавить ученика, который ходит в одной группе на ИЗО, а в другой на лепку?"))
    {
        draw_text("1. Добавьте ученика в обе группы");
        draw_text("2. Перейдите во вкладку 'Ученики'");
        draw_text("3. Для соответствующего ученика и соответствующих групп выберите ИЗО или лепку");
        get_images().edit_attend_data.draw();
        draw_note("Ученик может посещать ИЗО + ИЗО, Лепка + Лепка + Дизайн и любые другие комбинации");
    }
    if (ImGui::CollapsingHeader("Изменения от 26 окт."))
    {
        draw_text("1. Добавлен поиск по всем ученикам. ");
        get_images().student_search_1.draw();
        get_images().student_search_2.draw();
        draw_text("2. Добавлена возможность перемещать учеников в другую группу одной кнопкой.");
        draw_text("   Если ученик был перемещен таким способом, то он останется видимым (как удалённый) в исходной группе");
        get_images().move_to_group_1.draw();
        get_images().move_to_group_2.draw();
        get_images().move_to_group_3.draw();
        get_images().move_to_group_4.draw();
        draw_text("3. Разрешена отработка между ИЗО и Спецкурсом");
        draw_text("");
        draw_text("Исправления: ");
        draw_text("1. Теперь невозможно открыть второе окно программы");
        draw_text("2. Исправлена загрузка цен на новый месяц");
        draw_text("3. Теперь в списке групп, в которых находится ученик, нет удалённых групп");
    }
    draw_text("");
    draw_text("BUILD " + std::string(__DATE__) + " " + std::string(__TIME__));
    draw_text("Renderer " + std::string(Impl::renderer()->name()));
    draw_text("OS " + std::string(Impl::platform()->name()));
    if (_button_recalculate_all_prices && ImGui::Button("[DEBUG] Пересчитать все цены за текущий месяц"))
    {
        /* dirty fix for already broken prices */
        for (int wday = 0; wday < 7; wday++)
        {
            auto mdays_with_info = journal->enumerate_days(wday);
            int merged_lesson_count = journal->lesson_info_count(wday);
            for (auto mday : mdays_with_info)
            {
                for (int merged_lesson_id = 0; merged_lesson_id < merged_lesson_count; merged_lesson_id++)
                {
                    auto lesson_info = journal->lesson_info(wday, merged_lesson_id);
                    for (int internal_lesson_id = 0; internal_lesson_id < lesson_info->get_lessons_size(); internal_lesson_id++)
                    {
                        Lesson lesson
                        {
                            .merged_lesson_id = merged_lesson_id,
                            .internal_lesson_id = internal_lesson_id
                        };
                        for (int internal_student_id = 0; internal_student_id < lesson_info->get_group().get_size(); internal_student_id++)
                        {
                            auto status = mday.day->get_status(lesson, internal_student_id);
                            bool workout_existed = false; // it only matters if workout was deleted. I do not want to delete anything.
                            journal->set_lesson_status(mday.number - MDAY_DIFF, lesson, internal_student_id, status, workout_existed);
                        }
                    }
                }
            }
        }
    }
    if (_button_open_workout_debugging && ImGui::Button("[DEBUG] Удалить поврежденные отработки за текущий месяц"))
    {
        popup_handler->open_popup(new Popup_Workout_Debugging(graphical));
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();
    return false;
}

void show_button_to_recalculate_all_prices()
{
    _button_recalculate_all_prices = true;
}

void show_button_to_open_workout_debugging()
{
    _button_open_workout_debugging = true;
}
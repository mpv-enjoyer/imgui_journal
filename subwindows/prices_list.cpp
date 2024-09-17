#include "prices_list.h"

Subwindow_Prices_List::Subwindow_Prices_List(Graphical *_graphical)
{
    graphical = _graphical;
    journal = &(graphical->journal);
    if (!Journal::get_default_prices(prices, price_ill, price_skipped))
    {
        price_loaded_from_journal = true;
        prices = journal->get_lesson_prices();
        price_ill = journal->get_ill_price();
        price_skipped = journal->get_skipped_price();
    }
}

bool Subwindow_Prices_List::show_frame()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("Задать цены для следующих месяцев", nullptr, WINDOW_FLAGS | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    if (ImGui::Button("Вернуться к журналу"))
    {
        ImGui::End();
        return true;
    }

    if (price_loaded_from_journal)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "Файл со стандартными ценами не найден. Цены загружены из текущего месяца журнала.");
    }

    if (unsaved_changes)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "Есть несохраненные изменения.");
    }

    if (ImGui::InputInt("Цена для пропуска без уважительной причины", &price_skipped)) unsaved_changes = true;
    if (ImGui::InputInt("Цена для пропуска с уважительной причиной", &price_ill)) unsaved_changes = true;

    if (ImGui::Button("Добавить уровень скидки"))
    {
        unsaved_changes = true;
        for (int i = 0; i < LESSON_TYPE_COUNT; i++)
        {
            prices[i].push_back(*(prices[i].end() - 1));
        }
    }

    if (prices[0].size() > 1)
    {
        ImGui::SameLine();
        if (ImGui::Button("Убрать уровень скидки"))
        {
            unsaved_changes = true;
            for (int i = 0; i < LESSON_TYPE_COUNT; i++)
            {
                prices[i].erase(prices[i].end() - 1);
            }
        }
    }

    for (int i = 0; i < LESSON_TYPE_COUNT; i++)
    {
        ImGui::Separator();
        ImGui::Text((journal->Lesson_name(i) + ":").c_str());
        for (int j = 0; j < prices[0].size(); j++)
        {
            ImGui::TextDisabled("Цена для %i уч.:", j + 1);
            ImGui::SameLine();
            std::string label = generate_label("##input price", {i, j});
            if (ImGui::InputInt(label.c_str(), &(prices[i][j]))) unsaved_changes = true;
        }
    }

    if (j_button_colored("Сохранить", 0.1f, 0.9f, 0.1f))
    {
        Journal::set_default_prices(prices, price_ill, price_skipped);
        if (Journal::get_default_prices(prices, price_ill, price_skipped))
        {
            unsaved_changes = false;
            price_loaded_from_journal = false;
            ImGui::End();
            return true;
        }
    }

    ImGui::End();
    return false;
}

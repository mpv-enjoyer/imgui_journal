#pragma once
#include "view/ui/ui.h"
#include "model/imodel.h"
#include "controller/icontroller.h"
#include "view/elements/elements.h"
#include "controller/commands/commands.h"
#include "view/shared.h"

namespace View
{
    class Popup
    {
        const Shared& m_shared;
        std::string m_id;
        IController& m_controller;

        class
        {
            std::string m_error;
        public:
            void set(std::string error)
            {
                m_error = "ошибка: " + error;
            }
            void print() const
            {
                UI::label(m_error, {0.9f, 0.1f, 0.1f, 1.0f});
            }
        } m_last_error;

        class
        {
            bool m_any_item_active = false;
            int m_popup_count = 1;
            bool m_popup_count_became_one = false;
        public:
            void update_counters()
            {
                m_popup_count_became_one = m_popup_count != 1 && ImGui::GetPopupCount() == 1;
                m_popup_count = ImGui::GetPopupCount();
                m_any_item_active = ImGui::IsAnyItemActive();
            }
            bool should_exit() const
            {
                return m_popup_count == 1 && !m_popup_count_became_one && !m_any_item_active && ImGui::IsKeyPressed(ImGuiKey_Escape);
            }
        } m_esc_detector;

        std::optional<std::string> check_all_errors() const
        {
            // Popup error, definitely should be shown to user (or controller error by default):
            if (auto error = get_error()) return error;
            for (auto& action : get_actions())
            {
                if (auto error = action->get_error(model())) return error;
            }
            return {};
        }
    protected:
        const IModel& model() const { return m_controller.model(); }
        const Shared& shared() const { return m_shared; }
        virtual bool render_logic() = 0;
        virtual std::vector<std::shared_ptr<ICommand>> get_actions() const = 0;
        virtual std::optional<std::string> get_error() const = 0;
    public:
        Popup(const Shared& shared, std::string id, IController& controller)
        : m_shared(shared), m_id(id), m_controller(controller) { }
        bool render()
        {
            ImGui::OpenPopup(m_id.c_str());
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            m_esc_detector.update_counters();
            if (!ImGui::BeginPopupModal(m_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
            {
                return false;
            }
            bool popup_active = true;
            if (!render_logic()) popup_active = false;
            if (m_esc_detector.should_exit()) popup_active = false;
            if (ImGui::Button("OK"))
            {
                if (auto error = check_all_errors())
                {
                    m_last_error.set(*error);
                }
                else
                {
                    for (auto& action : get_actions())
                    {
                        m_controller.add(std::move(action));
                    }
                    popup_active = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Отмена")) popup_active = false;
            ImGui::SameLine();
            m_last_error.print();
            if (!popup_active) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return !popup_active;
        }
    };
}
#pragma once
#include "ui/ui.h"
#include "model/imodel.h"
#include "controller/icontroller.h"

namespace View
{
    class Popup
    {
        std::string m_last_error = "";
        bool m_any_item_active = false;
        int m_popup_count = 1;
        bool m_popup_count_became_one = false;
        std::string m_id;
        IController& m_controller;
        void update_counters_for_esc()
        {
            m_popup_count_became_one = m_popup_count != 1 && ImGui::GetPopupCount() == 1;
            m_popup_count = ImGui::GetPopupCount();
            m_any_item_active = ImGui::IsAnyItemActive();
        }
        bool should_exit_using_esc() const
        {
            return m_popup_count == 1 && !m_popup_count_became_one && !m_any_item_active && ImGui::IsKeyPressed(ImGuiKey_Escape);
        }
        void print_error()
        {
            ImGui::TextColored({0.9f, 0.1f, 0.1f, 1.0f}, m_last_error.c_str());
        }
    protected:
        const IModel& m_model;
        virtual bool render_logic() = 0;
        virtual void accept_changes(IController& controller) = 0;
        virtual std::optional<std::string> get_error(const IController& controller) = 0;
    public:
        Popup(std::string id, IController& controller, const IModel& model)
        : m_id(id), m_controller(controller), m_model(model)
        {
        
        }
        bool render()
        {
            ImGui::OpenPopup(m_id.c_str());
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            update_counters_for_esc();
            if (!ImGui::BeginPopupModal(m_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
            {
                return false;
            }
            bool popup_active = true;
            if (!render_logic()) popup_active = false;
            if (should_exit_using_esc()) popup_active = false;
            if (ImGui::Button("OK"))
            {
                auto error = get_error(m_controller);
                if (error)
                {
                    m_last_error = *error;
                }
                else
                {
                    accept_changes(m_controller);
                    popup_active = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Отмена")) popup_active = false;
            ImGui::SameLine();
            print_error();
            if (!popup_active) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return popup_active;
        }
};
}
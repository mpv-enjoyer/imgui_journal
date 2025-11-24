#pragma once
#include "aunit.h"
#include <functional>
#include <vector>
#include "model/lesson_infos.h"
#include "helpers.h"

namespace UI
{
    template <typename T>
    class Combobox : public AUnit
    {
    public:
        struct Choice
        {
            std::string description;
            T value;
            Choice(std::string description_, T value_)
            : description(description_), value(value_)
            { }
        };
    private:
        std::vector<Choice> m_choices;
    	int m_value;
    	std::function<bool(T)> m_callback;
        std::vector<char> m_choices_combo_text;
    public:
        Combobox(std::string id, std::vector<Choice> choices, std::function<bool(T)> callback = nullptr)
        : AUnit(id), m_choices(choices), m_value(0), m_callback(callback)
        {
            for (auto choice : choices)
            {
                for (const char c : choice.description)
                {
                    m_choices_combo_text.push_back(c);
                }
                m_choices_combo_text.push_back('\0');
            }
            m_choices_combo_text.push_back('\0');
        }
        void render_logic() override
        {
            Scope_Color_Input color;
            int value_buffer = m_value;
            if (!ImGui::Combo(m_id.c_str(), &value_buffer, m_choices_combo_text.data())) return;
            if (!m_callback || m_callback(m_choices[value_buffer].value)) m_value = value_buffer;
        }
        T get_choice() const
        {
            return m_choices[m_value].value;
        }
    };

    template <typename T>
    using Combobox_Choice = typename Combobox<T>::Choice;
    template <typename T>
    using Combobox_Choices = std::vector<typename Combobox<T>::Choice>;
}
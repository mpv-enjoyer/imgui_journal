//WIP DO NOT USE

#include <imgui.h>
#include <imgui_internal.h>
#include <stdint.h>
#include <imgui_widgets.cpp>
#include <string>

namespace JCombo
{
    bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags, int max_value_len);
    bool BeginComboPopup(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags);
    void EndCombo();
    bool BeginComboPreview();
    void EndComboPreview();
}

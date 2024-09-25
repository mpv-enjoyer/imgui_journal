#include "platforms.h"

// Linux: do nothing
bool impl::is_application_already_running()
{
    return false;
}

bool impl::load_font(ImGuiIO* io)
{
    if (!io->Fonts->AddFontFromFileTTF("segoeui.ttf", 18.0f, nullptr, io->Fonts->GetGlyphRangesCyrillic()))
    {
		return false;
    }
    return true;
}
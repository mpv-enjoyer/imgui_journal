#include "platforms.h"

// Linux: do nothing
bool Impl::CompiledPlatform::is_application_already_running()
{
    return false;
}

bool Impl::CompiledPlatform::load_font(ImGuiIO* io)
{
    if (!io->Fonts->AddFontFromFileTTF("segoeui.ttf", 18.0f, nullptr, io->Fonts->GetGlyphRangesCyrillic()))
    {
        return false;
    }
    return true;
}

const char* Impl::CompiledPlatform::name()
{
	return "Linux";
}
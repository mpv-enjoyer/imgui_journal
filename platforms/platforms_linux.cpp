#include "platforms.h"

// Linux: do nothing
bool Impl::CompiledPlatform::is_application_already_running()
{
    return false;
}

const char *Impl::CompiledPlatform::font_path()
{
    return "./segoeui.ttf";
}

const char* Impl::CompiledPlatform::name()
{
	return "Linux";
}
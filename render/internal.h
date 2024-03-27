#pragma once
#include "render.h"

#define DEFAULT_FRAME_DATA_MEMBER(T, N, V) \
    T _ ## N = V; \
    public:  \
    const T& N = std::ref(_ ## N); \
    bool increment_ ## N(); \
    bool reset_ ## N() { _ ## N = V; }; \
    private:

class Frame_Data
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
public:
    Frame_Data();
};

static bool _edit_mode;
static int _wday;
static std::vector<Day_With_Info> _visible_days;
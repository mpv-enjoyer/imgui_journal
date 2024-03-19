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
    DEFAULT_FRAME_DATA_MEMBER(int, visible_day_id, 0)
    DEFAULT_FRAME_DATA_MEMBER(int, merged_lesson, 0)
    DEFAULT_FRAME_DATA_MEMBER(int, internal_lesson, 0)
    DEFAULT_FRAME_DATA_MEMBER(int, internal_student_id, 0)
    DEFAULT_FRAME_DATA_MEMBER(int, visible_student_counter, 0)
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
public:
    Frame_Data();
};


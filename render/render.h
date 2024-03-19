#pragma once
#include "../main_types/main_types.h"
#include "../popups/popups.h"
#include "../subwindows/subwindows.h"
#include "../storage/public.h"

class Frame_Data
{
    int merged_lesson;
    int internal_lesson;
    int internal_student_id;
    int internal_student_counter;
};

namespace Render
{
    void show_frame();
}
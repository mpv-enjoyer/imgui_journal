#pragma once
#include "main.h"
#include "popups/popups.h"
#include "helpers/helpers.h"
#include "main_types/main_types.h"
#include "subwindows/subwindows.h"

void fill_NAW_in_calendar(std::vector<Visible_Day>* visible_days, int known_merged_lesson_id, Attend_Data new_attend_data, int known_student_id);
#pragma once
#include "../main.h"
#include "../helpers/helpers.h"
NO_IMPLICIT_CONVERSION_T(int, InternalLessonID);
#define INTERNAL_LESSON_FIRST ((InternalLessonID)0)
#define INTERNAL_LESSON_SECOND ((InternalLessonID)1)
#include "student.h"
#include "group.h"
#include "lesson_info.h"
#include "calendar_day.h"
#include "workout_info.h"
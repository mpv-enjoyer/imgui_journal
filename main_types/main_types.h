#pragma once
#include "../main.h"
#include "../helpers/helpers.h"
NO_IMPLICIT_CONVERSION_T(std::size_t, InternalLessonID);
#define INTERNAL_LESSON_FIRST ((InternalLessonID)0)
#define INTERNAL_LESSON_SECOND ((InternalLessonID)1)
#include "student.h"
#include "students.h"
#include "group.h"
#include "lesson_info.h"
#include "lessons_day.h"
#include "calendar_day.h"
#include "workout_info.h"
#include "workout_info.h"

void attempt()
{
    std::unordered_set<Workout_Info_Ptr, Real_Workout_Hash> r;
    Workout_Info j;
    Workout_Info_Ptr f = (Workout_Info_Ptr)&j;
    r.insert(f);
}
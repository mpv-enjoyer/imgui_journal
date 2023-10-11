# imgui_journal

- ImGui::BeginCombo(...) modified to calculate width better

- jcombo is meant to replace original combo to calculate MAX value width.

- main.h file is dumb and needs to be renamed

TODO:
- Restrict all edits for future days (including workouts)
- Fix adding workouts for current day
- Edit mode: remove student and change attendance for student
- Remove lesson_ignore_ids -> store this data in the lesson itself.
- All_Groups edit.
- additional shortcuts for editing
- saving/importing everything to a file
- cross-month actions

BUGS:
- removed students from groups should not be actually removed (because their attendance dissappears)
- workout is possible to set for a non-learning student (probably fixed)
- workout day_of_the_week is wrong (Среда)
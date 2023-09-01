# imgui_journal

- ImGui::BeginCombo(...) modified to calculate width better

- jcombo is meant to replace original combo to calculate MAX value width.

- main.h file is dumb and needs to be renamed

TODO:
- switching month doesn't actually change anything in the all_days[] thing itself.
- Workouts.
- All_Groups edit.
- additional shortcuts for editing
- saving/importing everything to a file
- cross-month actions

BUGS:
- add lesson, if begin_lesson_time > end_lesson_time, everything crashes (probably implement exceptions)
- removed students from groups should not be actually removed (because their attendance dissappears)
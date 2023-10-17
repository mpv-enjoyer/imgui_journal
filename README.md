# imgui_journal

- ImGui::BeginCombo(...) modified to calculate width better

- jcombo is meant to replace original combo to calculate MAX value width.

- main.h file is dumb and needs to be renamed

TODO:
- gray out every STATUS_NOT_AWAITED
- removed students from groups should not be actually removed (because their attendance dissappears)
- Restrict all edits for future days (including workouts)
- Remove lesson_ignore_ids -> store this data in the lesson itself.
- Fix table sizing
- All_Groups edit.
- additional shortcuts for editing
- saving/importing everything to a file
- cross-month actions

BUGS:

none known currently

Structural TODO:
- Add hash table lookup instead of manually checking everything (probably done already as my == operators work on reference addresses)
- Add overloads for Calendar_Day::get_status etc... for id input (must be much faster)
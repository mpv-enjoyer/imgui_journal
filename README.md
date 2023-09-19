# imgui_journal

- ImGui::BeginCombo(...) modified to calculate width better

- jcombo is meant to replace original combo to calculate MAX value width.

- main.h file is dumb and needs to be renamed

TODO:
- Workouts.
- All_Groups edit.
- additional shortcuts for editing
- saving/importing everything to a file
- cross-month actions

BUGS:
IMPORTANT: there's a horrendous bug. It happens randomly and it never happened during GDB debugging sessions. Old versions of this program are affected.
My guess is that some library isn't loading properly.

- removed students from groups should not be actually removed (because their attendance dissappears)
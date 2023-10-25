# imgui_journal

TODO:
- gray out every STATUS_NOT_AWAITED
- removed students from groups should not be actually removed (because their attendance dissappears)
- Fix table sizing
- All_Groups edit.
- additional shortcuts for editing
- saving/importing everything to a file
- cross-month actions

BUGS: 0 found.

Structural TODO:
- Add hash table lookup instead of manually checking everything (probably done already as my == operators work on reference addresses)
^ nah, I need those sorted vectors.
- use glfw_wait_events() instead! Should be a huge thing 
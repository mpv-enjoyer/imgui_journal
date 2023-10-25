# imgui_journal

NOTES:
- arguments started with known_ require you to know the location of the item in the internal vector.

TODO:
- gray out every STATUS_NOT_AWAITED
- removed students from groups should not be actually removed (because their attendance dissappears)
- Fix table sizing
- All_Groups edit.
- additional shortcuts for editing
- saving/importing everything to a file
- cross-month actions

BUGS:
- ANTIGDB: crashes while adding lesson and group. Repro: "Четверг, создать урок ИЗО 00:00 - 01:00, номер группы не важен, описание не важно"
- no O buttons in edit_mode

Structural TODO:
- Add hash table lookup instead of manually checking everything (probably done already as my == operators work on reference addresses)
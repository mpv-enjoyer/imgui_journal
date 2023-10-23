# imgui_journal

NOTES:
- arguments started with known_ require you to know the location of the item in the internal vector.

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

BUGS: none known currently

Structural TODO:
- Add hash table lookup instead of manually checking everything (probably done already as my == operators work on reference addresses)
- Add overloads for Calendar_Day::get_status etc... for id input (must be much faster)

RANT:
honestly, this branch is such a disappointment. First it started with thoughts about referencing objects in order to not use all_*** vectors.
But it turned out so badly, I just can't see the pros anymore. 
- Now I have multiple overloads which I may never use.
- Now my saving process is going to be more painful and I'm going to assign ID's at the end anyway.
- My codebase actually grew for every single file I tried to fix.
- In some cases it's still impossible to use the new system and I have to give all_*** to some methods anyway!
Future ideas? I will probably make some class like Globals.
This is dumb too but I have to admit that I either make a working program or I try to make it look as good as possible in the code editor.
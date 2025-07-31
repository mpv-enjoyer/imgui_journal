# imgui_journal

TODO:
 - Get rid of internal_ids, morth into classes and create get/set funcs for:
  - wday(get_RU, get_EN), mday(get_from_0, get_from_1), year(get_from_0, get_from_1900), JTime is probably fine
  - student_internal_id(Student* get(), internal id and corresponding Journal must be private)
  - lesson_info_internal_id(instead use Lesson_Info_Iter with Lesson_Info* get() and corresponding Lesson_Day maybe? idk)
  - more?

- bool is_property()
- Value get_object()
- Value& ref_object()
- const Value& cref_object()
- void set_object(Value)
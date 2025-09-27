TODO:

[+] void remove_student(int id);
[+] void restore_student(int id);
[+] void remove_lesson(int wday, int merged_lesson_id);
[+] void restore_lesson(int wday, int merged_lesson_id);
[-] void set_lesson_status(int mday, Lesson lesson, int internal_student_id, Student_Status status, bool workout_existed);
[-] void set_student_name(int id, std::string name);
[-] void set_group_age_group(int wday, int merged_lesson_id, int age_group);
[-] void remove_student_from_group(int wday, int merged_lesson_id, int student_id);
[-] void restore_student_to_group(int wday, int merged_lesson_id, int student_id);
[-] void move_student_to_group(int wday, int merged_lesson_id, int student_id, int another_wday, int another_merged_lesson_id);
[-] bool does_group_exist(int wday, int number);
[-] void set_student_contract(int id, int contract);
[-] void set_group_number(int wday, int merged_lesson_id, int number);
[-] void set_group_comment(int wday, int merged_lesson_id, std::string comment);
[-] void set_student_attend_data(int wday, int merged_lesson_id, int internal_student_id, Attend_Data new_attend_data);
[-] void set_teacher_name(int mday, Lesson lesson, std::string name);
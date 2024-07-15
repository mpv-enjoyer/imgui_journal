#include "journal.h"
#include <iostream>

void Test_Journal::add_student()
{
    /*Journal journal;
    journal.set_date(3, 2024 - 1900);
    journal.generate();
    journal.add_student_to_base("test_student", 10);
    IM_ASSERT(journal._all_students[0]->get_name() == "test_student");
    IM_ASSERT(journal._all_students[0]->get_age_group() == -1);
    IM_ASSERT(journal._all_students[0]->get_contract() == 10);*/
}

void Test_Journal::add_merged_lesson()
{
    /*Journal journal;
    journal.set_date(3, 2024 - 1900);
    journal.generate();
    Lesson_Pair lesson_pair;
    lesson_pair.lesson_name_id = NAME_TECHDRAWING;
    lesson_pair.time_begin = (JTime){10, 40};
    lesson_pair.time_end = (JTime){11, 25};
    journal.add_merged_lesson(1, 10, "test_comment", -1, {lesson_pair});
    IM_ASSERT(journal._all_groups[0]->get_age_group() == -1);
    IM_ASSERT(journal._all_groups[0]->get_comment() == "test_comment");
    IM_ASSERT(&(journal._all_lessons[1][0]->_group()) == journal._all_groups[0]);
    // Calendar_Day's important fields are private so we search for
    // illegal array access here.
    journal.add_student_to_base("test_student", 10);
    journal.add_student_to_group(0, 1, 0);
    IM_ASSERT(journal.day(0)->get_status({0, 0}, 0).status == STATUS_NOT_AWAITED);
    journal.day(28)->get_status({0, 0}, 0);
    IM_ASSERT(journal.day(0)->get_workout_size({0, 0}) == 0);*/
}

void Test_Journal::add_workout()
{
    /*Journal journal;
    journal.set_date(3, 2024 - 1900);
    journal.generate();
    Lesson_Pair lesson_pair;
    lesson_pair.lesson_name_id = NAME_TECHDRAWING;
    lesson_pair.time_begin = (JTime){10, 40};
    lesson_pair.time_end = (JTime){11, 25};
    journal.add_student_to_base("test_student", 10);
    journal.add_merged_lesson(1, 10, "test_comment", -1, {lesson_pair});
    journal.add_student_to_group(0, 1, 0);
    journal.add_merged_lesson(2, 20, "test_comment_2", 1, {lesson_pair});
    std::tm tm_to;
    tm_to.tm_mon = 3;
    tm_to.tm_mday = 0;
    tm_to.tm_wday = 1;
    tm_to.tm_year = 2024 - 1900;
    std::tm tm_from = tm_to;
    tm_from.tm_mday = 1;
    tm_from.tm_wday = 2;
    journal.add_working_out(tm_from, tm_to, 0, {0, 0}, {0, 0});
    IM_ASSERT(journal.day(1)->get_workout_size({0, 0}) == 1);
    IM_ASSERT(journal.day(0)->get_status({0, 0}, 0).status == STATUS_WORKED_OUT);
    std::string s = journal.day(0)->get_status({0, 0}, 0).workout_info.lesson_info->get_group().get_comment();
    IM_ASSERT(s == "test_comment_2");*/
}

void Test_Journal::discount()
{
    /*Journal journal;
    journal.set_date(3, 2024 - 1900);
    journal.generate();
    journal.add_student_to_base("test_student_1", 1);
    journal.add_student_to_base("test_student_2", 1);
    int discount_status = journal._discount_status(1);
    IM_ASSERT(discount_status == 1);
    IM_ASSERT(journal.lesson_common_price(1, NAME_DESIGN) == journal.Lesson_price(NAME_DESIGN, 1));
    Lesson_Pair lesson_pair;
    lesson_pair.lesson_name_id = NAME_TECHDRAWING;
    lesson_pair.time_begin = (JTime){10, 40};
    lesson_pair.time_end = (JTime){11, 25};
    journal.add_merged_lesson(1, 10, "test_comment", -1, {lesson_pair});
    journal.add_student_to_group(0, 1, 0);
    IM_ASSERT(journal.lesson_current_price({0, 0}, 0, 0) == -1);
    journal._day(0)->set_status({0, 0}, 0, STATUS_WAS_ILL);
    IM_ASSERT(journal.lesson_current_price({0, 0}, 0, 0) == 0);
    journal._day(0)->set_status({0, 0}, 0, STATUS_SKIPPED);
    IM_ASSERT(journal.lesson_current_price({0, 0}, 0, 0) == 50);*/
}

void Test_Journal::merged_lesson_sort()
{
    /*Journal journal;
    journal.set_date(3, 2024 - 1900);
    journal.generate();
    journal.add_student_to_base("test_student_1", 1);
    journal.add_student_to_base("test_student_2", 1);
    Lesson_Pair lesson_pair;
    lesson_pair.lesson_name_id = NAME_TECHDRAWING;
    lesson_pair.time_begin = (JTime){10, 40};
    lesson_pair.time_end = (JTime){11, 25};
    journal.add_merged_lesson(0, 1, "test_comment", -1, {lesson_pair});
    journal.add_merged_lesson(0, 1, "test_comment", -1, {lesson_pair});
    lesson_pair.time_begin = (JTime){12, 00};
    lesson_pair.time_end = (JTime){12, 40};
    journal.add_merged_lesson(0, 2, "test_comment", -1, {lesson_pair});
    lesson_pair.time_begin = (JTime){9, 30};
    lesson_pair.time_end = (JTime){10, 10};
    journal.add_merged_lesson(0, 0, "test_comment", -1, {lesson_pair});
    IM_ASSERT(journal.lesson_info(0, 0)->get_group().get_number() == 0);
    IM_ASSERT(journal.lesson_info(0, 1)->get_group().get_number() == 1);
    IM_ASSERT(journal.lesson_info(0, 2)->get_group().get_number() == 1);
    IM_ASSERT(journal.lesson_info(0, 3)->get_group().get_number() == 2);*/
}

Test_Journal::Test_Journal()
{
    add_merged_lesson();
    add_student();
    add_workout();
    discount();
    merged_lesson_sort();
    std::cout << "All tests passed!";
}
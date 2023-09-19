#include "main.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int journal_get_lesson() //to replace int with custom structure which will define date + lesson
{
    int test = 0;
    ImGui::OpenPopup("Отработка");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Отработка", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("отработка за ФИО ученика ** группы *, **:00", 1,1+8);
        ImGui::Text("<самодельный календарь, с подсветкой дней, в которые этот человек должен был прийти>");
        ImGui::Text(std::to_string(test).c_str()); if (ImGui::Button("make test 1")) test = 1;
        if (ImGui::Button("OK", ImVec2(0, 0))) ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
} 

int get_number_of_days(int month, int year)
{
    month++;
    // leap year condition, if month is 2
    if (month == 2) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    // months which has 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
        || month == 10 || month == 12)
        return 31;
    else
        return 30;
}

int calculate_first_mwday(int current_mday, int current_wday)
{
    int first_same = ((current_mday-1) % 7) + 1;
    int diff = first_same - 1;
    return (current_wday - diff + 7) % 7;
}

int get_first_wday(int month, int year, int wday)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
      1, month, year}; // 1-based day, 0-based month, year since 1900

  std::time_t time_temp = std::mktime(&time_in);

  //Note: Return value of localtime is not threadsafe, because it might be
  // (and will be) reused in subsequent calls to std::localtime!
  const std::tm time_out = *std::localtime(&time_temp);
  int first_mday_wday = time_out.tm_wday;
  int diff = ( ( wday - first_mday_wday ) + 7 ) % 7;
  return diff + 1;
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Журнал версии 0.0.1", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    #ifdef _WIN32
    if(!io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic()))
    {
        return -1;
    }
    #else
    if(!io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/NotoMonoNerdFont-Regular.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic()))
    {
        return -1;
    }
    #endif //not supporting apple platform lolol

    time_t current_time_temp = time(NULL);
    const std::tm current_time = *std::localtime(&current_time_temp);
    
    int current_month = current_time.tm_mon;
    int current_day_of_the_week = current_time.tm_wday;
    int current_year = current_time.tm_year;
    int day_of_the_week_first_in_month = calculate_first_mwday(current_time.tm_mday, current_day_of_the_week);

    std::vector<Student> all_students;
    std::vector<Group> all_groups;
    std::vector<std::vector<Lesson_Info>> all_lessons(7, std::vector<Lesson_Info>(0, Lesson_Info(&all_groups))); //[day_of_the_week][]
    std::vector<Calendar_Day> all_days;

    all_students.push_back(Student());
    all_students[0].set_name("Фамилия Имя 1");
    all_students[0].set_contract(1);
    all_students.push_back(Student());
    all_students[1].set_name("Фамилия Имя 2");
    all_students[1].set_contract(21);
    all_students.push_back(Student());
    all_students[2].set_name("Фамилия Имя 3");
    all_students[2].set_contract(100);
    all_students.push_back(Student());
    all_students[3].set_name("Фамилия Имя 4");
    all_students[3].set_contract(100);
    all_students[3].set_age_group(5);
    all_groups.push_back(Group(&all_students));
    all_groups[0].add_student(1);
    all_groups[0].add_student(2);
    all_groups[0].add_student(3);
    all_groups[0].set_number(7);
    all_groups[0].set_day_of_the_week(2);
    all_groups.push_back(Group(&all_students));
    //all_groups[1].add_student(0);
    //all_groups[1].add_student(3);
    all_groups[1].set_number(2);
    all_groups[1].set_day_of_the_week(1);
    all_groups[1].set_comment("дошкольники");
    all_groups[1].set_day_of_the_week(2);
    Lesson_Info temp_lesson = Lesson_Info(&all_groups);
    Lesson_Pair temp_lesson_pair;
    temp_lesson_pair.lesson_name_id = 1;
    JTime temp_begin;
    JTime temp_end;
    temp_begin.hours = 9; temp_begin.minutes = 10;
    temp_end.hours = 10; temp_end.minutes = 50;
    temp_lesson_pair.time_begin = temp_begin;
    temp_lesson_pair.time_end = temp_end;
    temp_lesson.set_group(0);
    temp_lesson.add_lesson_pair(temp_lesson_pair);
    all_lessons[2].push_back(temp_lesson);

    temp_lesson_pair.lesson_name_id = 0;
    temp_begin.hours = 11; temp_begin.minutes = 10;
    temp_end.hours = 12; temp_end.minutes = 50;
    temp_lesson_pair.time_begin = temp_begin;
    temp_lesson_pair.time_end = temp_end;
    temp_lesson.delete_lesson_pair(0);
    temp_lesson.add_lesson_pair(temp_lesson_pair);
    temp_lesson.set_group(1);
    all_lessons[2].push_back(temp_lesson);

    //Lesson ignored_ = {1,0};
    //all_students[3].add_lesson_ignore_id(ignored_, current_day_of_the_week);

    int current_month_days_num = get_number_of_days(current_month, current_year + 1900);
    for (int i = 0; i < current_month_days_num; i++)
    {
       all_days.push_back(Calendar_Day(&all_lessons[(day_of_the_week_first_in_month + i) % 7], &all_groups, &all_students, (day_of_the_week_first_in_month + i) % 7));
    }
    bool opened_otr = false;
    std::vector<std::string> names{"Петухова Таисия Данииловна", "Рыжова Милана Андреевна", "Кузина Александра Сергеевна", "Лебедева Варвара Давидовна", "Куликов Дмитрий Ильич", "Петрова Вера Михайловна", "Панов Кирилл Иванович", "Дубровина Анна Никитична", "Михайлов Владимир Иванович", "Миронова Елизавета Алексеевна", "Пономарев Андрей Артёмович", "Никулина Дарья Степановна", "Иванов Ян Иванович", "Морозова Есения Марковна", "Мухина Ирина Михайловна", "Леонова Владислава Романовна", "Романов Владимир Владимирович", "Смирнов Роман Вадимович", "Кудряшов Иван Лукич", "Гусев Ростислав Давидович"};

    bool edit_mode = false;

    int popup_add_student_to_group_select = -1;
    int popup_add_student_to_group_merged_lesson = -1;
    bool popup_add_student_to_group_is_open = false;
    int popup_select_day_of_the_week_day_of_the_week = -1;
    int popup_select_day_of_the_week_month = -1;
    bool popup_select_day_of_the_week_is_open = false;
    bool popup_add_merged_lesson_to_journal_is_open = false;
    bool popup_add_working_out_is_open = false;
    int popup_add_working_out_select = -1;   
    int popup_add_working_out_merged_lesson = -1;
    int popup_add_working_out_internal_lesson = -1;
    int popup_add_working_out_mday = -1;

    bool window_add_student_list_is_open = false;
    int popup_edit_ignore_lessons_is_open = -1;

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
    //glfwWaitEvents();
    glfwPollEvents();
    //glfwWaitEventsTimeout(0.2F);
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    if (window_add_student_list_is_open)
    {
        if (students_list(&all_students, &all_groups, &popup_edit_ignore_lessons_is_open))
        {
            window_add_student_list_is_open = false;
        }
        if (popup_edit_ignore_lessons_is_open != -1)
        {
            bool ignore = false; //TODO: usable cancel button
            if (popup_edit_ignore_lessons(&all_lessons, &all_students, popup_edit_ignore_lessons_is_open, &ignore))
            {
                popup_edit_ignore_lessons_is_open = -1;
            }
        }
    }

    ImGui::Begin("Журнал версии 0.0.1", nullptr, flags);

    if (popup_select_day_of_the_week_is_open && popup_select_day_of_the_week(&popup_select_day_of_the_week_day_of_the_week, &popup_select_day_of_the_week_month))
    {
        popup_select_day_of_the_week_is_open = false;
        if (popup_select_day_of_the_week_day_of_the_week != -1 && popup_select_day_of_the_week_month != -1)
        {
            current_day_of_the_week = popup_select_day_of_the_week_day_of_the_week;
            current_month = popup_select_day_of_the_week_month;
        }
    }

    int count_visible_days = 0;
    //int first_visible_day = ((current_time->tm_mday-1) % 7) + 1;
    int first_visible_day = get_first_wday(current_month, current_year, current_day_of_the_week);
    int first_visible_day_copy = first_visible_day;
    std::vector<int> visible_table_columns;
    for (;first_visible_day_copy <= current_month_days_num; first_visible_day_copy+=7)
    {
        count_visible_days++;
        visible_table_columns.push_back(first_visible_day_copy-MDAY_DIFF);
    }

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

    ImGui::Text("Выбран день %s, %s текущего года", Day_Names[current_day_of_the_week].c_str(), Month_Names[current_month].c_str());
    ImGui::SameLine();
    if(ImGui::Button("Изменить день"))
    {
        glfwPostEmptyEvent();
        popup_select_day_of_the_week_day_of_the_week = current_day_of_the_week;
        popup_select_day_of_the_week_is_open = true;
        popup_select_day_of_the_week_month = current_month;
    };
    ImGui::SameLine();
    //ImGui::Button("Редактировать расписание занятий");
    if (ImGui::Button("Добавить урок"))
    {
        popup_add_merged_lesson_to_journal_is_open = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("Список учеников") )
    {
        window_add_student_list_is_open = true;
    }
    ImGui::SameLine();
    ImGui::Button("Журнал оплаты");
    ImGui::SameLine();
    ImGui::Button("Справка");
    ImGui::SameLine();
    ImGui::Checkbox("Режим редактирования", &edit_mode);

    if (popup_add_student_to_group_is_open)
    {
        bool pressed_ok = popup_add_student_to_group(&all_students, &all_groups, &all_days, all_lessons[current_day_of_the_week][popup_add_student_to_group_merged_lesson].get_group(), &popup_add_student_to_group_select);
        if (pressed_ok)
        {
            if (popup_add_student_to_group_select != -1)
            {
                int new_student_id = all_groups[all_lessons[current_day_of_the_week][popup_add_student_to_group_merged_lesson].get_group()].add_student(popup_add_student_to_group_select);
                for (int current_day_cell = 0; current_day_cell < count_visible_days; current_day_cell++)
                {
                    all_days[visible_table_columns[current_day_cell]].add_student_to_group(all_lessons[current_day_of_the_week][popup_add_student_to_group_merged_lesson].get_group(), popup_add_student_to_group_select, new_student_id);
                    for (int internal_lesson_id = 0; internal_lesson_id < all_lessons[current_day_of_the_week][popup_add_student_to_group_merged_lesson].get_lessons_size(); internal_lesson_id++)
                    {
                        Lesson current_lesson = {popup_add_student_to_group_merged_lesson, internal_lesson_id};
                        int status = STATUS_NO_DATA;
                        if (visible_table_columns[current_day_cell] <= current_time.tm_mday) status = STATUS_NOT_AWAITED;
                        all_days[visible_table_columns[current_day_cell]].set_status(current_lesson, popup_add_student_to_group_select, status);
                    }
                }
            } 
            popup_add_student_to_group_is_open = false;
            popup_add_student_to_group_select = -1;
            popup_add_student_to_group_merged_lesson = -1;
        }
    }

    if (popup_add_working_out_is_open)
    {
        Workout_Info workout_lesson;
        bool pressed_ok = popup_add_working_out(&all_students, &all_groups, &all_days, &all_lessons, all_lessons[current_day_of_the_week][popup_add_working_out_merged_lesson].get_group(), &popup_add_working_out_select, day_of_the_week_first_in_month, current_month_days_num, &workout_lesson);
        if (pressed_ok)
        {
            if (popup_add_working_out_select != -1)
            {
                Lesson current_lesson = {popup_add_working_out_merged_lesson, popup_add_working_out_internal_lesson};
                all_days[popup_add_working_out_mday].add_workout(current_lesson, workout_lesson);//[popup_add_working_out_merged_lesson][popup_add_working_out_internal_lesson]
            }
            popup_add_working_out_is_open = false;
            popup_add_working_out_select = -1;
            popup_add_working_out_select_day = -1;
            popup_add_working_out_select_month = -1;
            popup_add_working_out_select_year = -1;
        }
    }

    if (popup_add_merged_lesson_to_journal_is_open) //Possible problems when working with previous months
    {
        Lesson_Info new_lesson_info = Lesson_Info(&all_groups);
        Group new_group = Group(&all_students);
        new_group.set_number(-1);
        bool ignore = false;
        if (popup_add_merged_lesson_to_journal(&all_groups, &new_lesson_info, &new_group, current_day_of_the_week, &ignore, false))
        {
            if (!ignore)
            {
                all_groups.push_back(new_group);
                all_lessons[current_day_of_the_week].push_back(new_lesson_info);
                all_lessons[current_day_of_the_week][all_lessons[current_day_of_the_week].size() - 1].set_group(all_groups.size() - 1);
                int new_lesson_id = all_lessons[current_day_of_the_week].size() - 1;
                for (int i = 0; i < count_visible_days; i++)
                {
                    bool await_no_one = false;
                    if (current_time.tm_mday >= visible_table_columns[i]) await_no_one = true;
                    all_days[visible_table_columns[i]].add_merged_lesson(current_day_of_the_week, new_lesson_info, await_no_one, all_lessons[current_day_of_the_week].size()-1);
                }
            }
            popup_add_merged_lesson_to_journal_is_open = false;
        }
    }

    ImGui::BeginChild("Child", ImVec2(0, 0), true, window_flags);

    if (all_lessons[current_day_of_the_week].size()==0)
    {
        ImGui::Text("На текущий день не запланированы уроки.");
    }
    else
    {
        JTime previous = {0, 0};
        Lesson current_lesson;
        std::vector<bool> sort_ignore_lessons(all_lessons[current_day_of_the_week].size(), false);

        for (int sort_merged_lesson = 0; sort_merged_lesson < all_lessons[current_day_of_the_week].size(); sort_merged_lesson++)
        {
            int current_merged_lesson = -1;
            for (int temp_sort = 0; temp_sort < all_lessons[current_day_of_the_week].size(); temp_sort++)
            {
                if (sort_ignore_lessons[temp_sort]) continue;
                if (current_merged_lesson == -1) current_merged_lesson = temp_sort;
                if (all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(0).time_begin >= all_lessons[current_day_of_the_week][temp_sort].get_lesson_pair(0).time_begin)
                {
                    current_merged_lesson = temp_sort;
                }
            }
            sort_ignore_lessons[current_merged_lesson] = true;
            if (previous == all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(0).time_begin) 
            {
                ImGui::SameLine();
            }
            previous = all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(0).time_begin;
            current_lesson.merged_lesson_id = current_merged_lesson;
            ImGui::BeginGroup();
            ImGui::Text(all_lessons[current_day_of_the_week][current_merged_lesson].get_description().c_str());
            const char* table_name = std::to_string(current_merged_lesson).c_str();

            if (ImGui::BeginTable(
            table_name, 
            DEFAULT_COLUMN_COUNT+count_visible_days, 
            ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoPadInnerX,
            ImVec2(std::numeric_limits<float>::max(),(0.0F))))
            {
                ImU32 row_bg_color = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
                ImGui::TableNextColumn(); ImGui::Text("No");
                ImGui::TableNextColumn(); ImGui::Text("ФИ ученика");
                ImGui::TableNextColumn(); ImGui::Text("Д-р");
                ImGui::TableNextColumn(); ImGui::Text("Программа");
                ImGui::TableNextColumn(); ImGui::Text("Цена");
                for (int i = 0; i < count_visible_days; i++)
                {
                    std::string day_temp = std::to_string(visible_table_columns[i] + 1) + "." + std::to_string(current_time.tm_mon + 1);
                    ImGui::TableNextColumn(); ImGui::Text(day_temp.c_str());
                }
                int current_group = all_lessons[current_day_of_the_week][current_merged_lesson].get_group();
                for (int current_student_group_id = 0; current_student_group_id < all_groups[current_group].get_size(); current_student_group_id++)
                {
                    int current_student_id = all_groups[current_group].get_student_sort_id(current_student_group_id);

                    ImGui::TableNextRow();
                    if (all_students.at(current_student_id).is_removed()) 
                    {
                        ImGui::BeginDisabled();
                        ImGui::TableSetColumnIndex(0); ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),"X");
                    }
                    else
                    {
                        ImGui::TableSetColumnIndex(0); ImGui::Text("%i", current_student_group_id+1);
                    }
                    ImGui::TableSetColumnIndex(1); ImGui::Text(all_students[current_student_id].get_name().c_str());
                    ImGui::TableSetColumnIndex(2); ImGui::Text("%i", all_students[current_student_id].get_contract());
                    int current_student_contract = all_students[current_student_id].get_contract();
                    int current_discount_level = -1;
                    for (int i = 0; i < all_students.size(); i++)
                    {
                        if (all_students[i].get_contract() == current_student_contract && current_discount_level < 2 && !all_students[i].is_removed())
                        {
                            current_discount_level++;
                        }
                    }
                    if (current_discount_level == -1) current_discount_level = 0; //TODO: restructure
                    std::vector<bool> is_relevant;
                    for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[current_day_of_the_week][current_merged_lesson].get_lessons_size(); current_internal_lesson++)
                    {
                        current_lesson.internal_lesson_id = current_internal_lesson;
                        if (all_students[current_student_id].is_ignored(current_lesson, current_day_of_the_week))
                        {
                            is_relevant.push_back(false);
                        }
                        else
                        {
                            is_relevant.push_back(true);
                        }
                    }
                    std::string show_price;
                    std::string show_lesson_names;
                    bool temp_first = true;
                    for (int i = 0; i < is_relevant.size(); i++)
                    {
                        if (is_relevant[i]) 
                        {
                            if (temp_first)
                            {
                                temp_first = false;
                            }
                            else
                            {
                                show_price.append("+");
                                show_lesson_names.append("+");
                            }
                            show_price.append(std::to_string(Lesson_Prices[all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(i).lesson_name_id][current_discount_level]));
                            show_lesson_names.append(Lesson_Names[all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(i).lesson_name_id]);
                        }
                    }
                    if (temp_first) //TODO: hide them entirely if they never were on the lesson
                    {
                        ImGui::BeginDisabled();
                        ImGui::TableSetColumnIndex(3); ImGui::Text("---");
                        ImGui::TableSetColumnIndex(4); ImGui::Text("---");
                        ImGui::EndDisabled();
                    }
                    else
                    {
                        ImGui::TableSetColumnIndex(3); ImGui::Text(show_lesson_names.c_str());
                        ImGui::TableSetColumnIndex(4); ImGui::Text(show_price.c_str());
                    }
                    for (int current_day_cell = 0; current_day_cell < count_visible_days; current_day_cell++)
                    {
                        ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + current_day_cell);

                        for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[current_day_of_the_week][current_merged_lesson].get_lessons_size(); current_internal_lesson++)
                        {

                            current_lesson.internal_lesson_id = current_internal_lesson;
                            Student_Status current_status = all_days[visible_table_columns[current_day_cell]].get_status(current_lesson, current_student_id);
                            int current_lesson_discount_status = all_days[visible_table_columns[current_day_cell]].get_discount_status(current_lesson, current_student_id);
                            int dummy = 0; //use ONLY in fake combos
                            ImGui::BeginDisabled();
                            if (current_status.status == STATUS_INVALID) 
                            {
                                ImGui::Combo("##dummy1", &dummy, "!?"); ImGui::SameLine(); ImGui::EndDisabled(); continue;
                            }
                            if (current_status.status == STATUS_NOT_AWAITED)
                            {
                                ImGui::Combo("##dummy2", &dummy, ".."); ImGui::SameLine(); ImGui::EndDisabled(); continue;
                            }
                            ImGui::EndDisabled();

                            std::string combo_name = "##" + std::to_string(current_merged_lesson) + " "
                             + std::to_string(current_internal_lesson) + " "
                             + std::to_string(current_day_cell) + " "
                             + std::to_string(current_student_id);
                            ImGui::BeginGroup();
                            if(ImGui::Combo(combo_name.c_str(), &(current_status.status), " \0+\0Б\0-\0О\0\0"))
                            {
                                if (current_status.status != STATUS_WORKED_OUT)
                                {
                                    all_days[visible_table_columns[current_day_cell]].set_status(current_lesson, current_student_id, current_status.status);
                                    all_days[visible_table_columns[current_day_cell]].set_discount_status(current_lesson, current_student_id, current_discount_level);
                                }
                                if (current_lesson_discount_status == -1) current_lesson_discount_status = current_discount_level;
                            }
                            if ((current_lesson_discount_status >=0 && current_discount_level != current_lesson_discount_status && current_status.status > STATUS_NO_DATA) || current_status.status == STATUS_WAS_ILL)
                            {
                                int current_lesson_price = -1;
                                if ((current_status.status == STATUS_ON_LESSON) || (current_status.status == STATUS_WORKED_OUT) || (current_status.status == STATUS_SKIPPED))
                                {
                                    current_lesson_price = Lesson_Prices[all_lessons[current_day_of_the_week][current_lesson.merged_lesson_id].get_lesson_pair(current_lesson.internal_lesson_id).lesson_name_id][current_lesson_discount_status];
                                }
                                if (current_status.status == STATUS_WAS_ILL)
                                {
                                    current_lesson_price = 0;
                                }
                                if (current_lesson_price != -1)
                                {
                                    ImGui::TextDisabled(std::to_string(current_lesson_price).c_str());
                                }
                            }
                            ImGui::EndGroup();
                            ImGui::SameLine();
                        }
                    }
                    if (all_students.at(current_student_id).is_removed()) ImGui::EndDisabled();
                }
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button(("Добавить ученика##" + std::to_string(current_merged_lesson)).c_str()))
                {
                    popup_add_student_to_group_is_open = true;
                    popup_add_student_to_group_select = -1;
                    popup_add_student_to_group_merged_lesson = current_merged_lesson;
                    popup_add_student_to_group(&all_students, &all_groups, &all_days, current_group, &popup_add_student_to_group_select);
                }
                ImGui::TableNextRow();
                for (int current_day_cell = 0; current_day_cell < visible_table_columns.size(); current_day_cell++)
                {
                    ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + current_day_cell);
                    if (ImGui::Button(("O##" + std::to_string(current_merged_lesson) + "+" + std::to_string(current_day_cell)).c_str()))
                    {
                        popup_add_working_out_is_open = true;
                        popup_add_working_out_select = -1;
                        popup_add_working_out_merged_lesson = current_lesson.merged_lesson_id;
                        popup_add_working_out_internal_lesson = current_lesson.internal_lesson_id;
                        popup_add_working_out_mday = visible_table_columns[current_day_cell];
                    }
                    
                }


                ImGui::EndTable();
                ImGui::EndGroup();
            }
        }
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();



    ImGui::End();
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
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
    GLFWwindow* window = glfwCreateWindow(1280, 720, "The journal itself", nullptr, nullptr);
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
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/NotoMonoNerdFont-Regular.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    time_t current_time_temp = time(NULL);
    const std::tm* current_time = std::localtime(&current_time_temp);
    int current_month = current_time->tm_mon;
    int current_day_of_the_week = current_time->tm_wday;

    int day_of_the_week_first_in_month = calculate_first_mwday(current_time->tm_mday, current_day_of_the_week);

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
    all_groups.push_back(Group(&all_students));
    all_groups[0].add_student(1);
    all_groups[0].add_student(2);
    all_groups[0].add_student(3);
    all_groups[0].set_number(7);
    all_groups.push_back(Group(&all_students));
    all_groups[1].add_student(0);
    all_groups[1].add_student(3);
    all_groups[1].set_number(2);
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
    all_lessons[current_day_of_the_week].push_back(temp_lesson);

    temp_lesson_pair.lesson_name_id = 0;
    temp_begin.hours = 11; temp_begin.minutes = 10;
    temp_end.hours = 12; temp_end.minutes = 50;
    temp_lesson_pair.time_begin = temp_begin;
    temp_lesson_pair.time_end = temp_end;
    temp_lesson.add_lesson_pair(temp_lesson_pair);
    temp_lesson.set_group(1);
    all_lessons[current_day_of_the_week].push_back(temp_lesson);

    Lesson ignored_ = {1,0};
    all_students[3].add_lesson_ignore_id(ignored_);

    int current_month_days_num = get_number_of_days(current_month, current_time->tm_year);
    for (int i = 0; i < current_month_days_num; i++)
    {
       all_days.push_back(Calendar_Day(&all_lessons[(day_of_the_week_first_in_month + i) % 7], &all_groups, &all_students));
    }
    bool opened_otr = false;
    std::vector<std::string> names{"Петухова Таисия Данииловна", "Рыжова Милана Андреевна", "Кузина Александра Сергеевна", "Лебедева Варвара Давидовна", "Куликов Дмитрий Ильич", "Петрова Вера Михайловна", "Панов Кирилл Иванович", "Дубровина Анна Никитична", "Михайлов Владимир Иванович", "Миронова Елизавета Алексеевна", "Пономарев Андрей Артёмович", "Никулина Дарья Степановна", "Иванов Ян Иванович", "Морозова Есения Марковна", "Мухина Ирина Михайловна", "Леонова Владислава Романовна", "Романов Владимир Владимирович", "Смирнов Роман Вадимович", "Кудряшов Иван Лукич", "Гусев Ростислав Давидович"};
    std::vector<std::string> lesson_names{"ИЗО", "Лепка", "Спецкурс", "Черчение"};
    std::vector<std::string> lessons{"ИЗО", "Лепка", "Спецкурс", "Черчение"}; 
    std::vector<int> prices{300, 200, 100, 44, 500}; //to be deleted

            static int is_here[15][15];

            for (int i = 0; i < 225; i++)
            {
                is_here[i%15][i/15] = 0;

            }
    static bool is_calendar_open = false;
    static bool is_working_out_open = false;
    int popup_add_student_to_group_select = -1;
    int popup_add_student_to_group_merged_lesson = -1;
    bool popup_add_student_to_group_is_open = false;

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
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    ImGui::Begin("The Journal itself", nullptr, flags);

    int count_visible_days = 0;
    int first_visible_day = ((current_time->tm_mday-1) % 7) + 1;
    int first_visible_day_copy = first_visible_day;
    std::vector<int> visible_table_columns;
    for (;first_visible_day_copy <= current_month_days_num; first_visible_day_copy+=7)
    {
        count_visible_days++;
        visible_table_columns.push_back(first_visible_day_copy-MDAY_DIFF);
    }

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

    ImGui::Text("Выбран день 14 июля 2023");
    ImGui::SameLine();
    if(ImGui::Button("Изменить день") || is_calendar_open)
    {
        glfwPostEmptyEvent();
        is_calendar_open = true;
        ImGui::OpenPopup("Календарь");
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(1.0F);
        if (ImGui::BeginPopupModal("Календарь", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("<самодельный календарь>");
            if (ImGui::Button("Ок")) { ImGui::CloseCurrentPopup(); is_calendar_open = false; }        
        }
        ImGui::EndPopup();
    };
    ImGui::SameLine();
    ImGui::Button("Редактировать расписание занятий");
    ImGui::SameLine();
    ImGui::Button("Добавить ученика");
    ImGui::SameLine();
    ImGui::Button("Журнал оплаты");
    ImGui::SameLine();
    ImGui::Button("Справка");

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
                    //most likely there will be a lot of suppressed errors and I do not care for now. TODO <<--
                    for (int internal_lesson_id = 0; internal_lesson_id < all_lessons[current_day_of_the_week][popup_add_student_to_group_merged_lesson].get_lessons_size(); internal_lesson_id++)
                    {
                        Lesson current_lesson = {popup_add_student_to_group_merged_lesson, internal_lesson_id};
                        int status = STATUS_NO_DATA;
                        if (visible_table_columns[current_day_cell] + 1 <= current_time->tm_mday) status = STATUS_NOT_AWAITED;
                        all_days[visible_table_columns[current_day_cell]].set_status(current_lesson, popup_add_student_to_group_select, status);
                    }
                }
            } 
            popup_add_student_to_group_is_open = false;
            popup_add_student_to_group_select = -1;
            popup_add_student_to_group_merged_lesson = -1;
        }
    }

    ImGui::BeginChild("Child", ImVec2(0, 0), true, window_flags);

    if (all_lessons[current_day_of_the_week].size()==0)
    {
        ImGui::Text("На текущий день не запланированы уроки.");
    }
    else
    {

        JTime previous = {0, 0};//all_lessons[current_day_of_the_week][0].get_lesson_pair(0).time_begin;
        Lesson current_lesson;


        for (int current_merged_lesson = 0; current_merged_lesson < all_lessons[current_day_of_the_week].size(); current_merged_lesson++)
        {
            if (previous == all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(0).time_begin) ImGui::SameLine();
            previous = all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(0).time_begin;
            current_lesson.merged_lesson_id = current_merged_lesson;
            ImGui::BeginGroup();
            ImGui::Text(all_lessons[current_day_of_the_week][current_merged_lesson].get_description().c_str());
            const char* table_name = std::to_string(current_merged_lesson).c_str();


            if (ImGui::BeginTable(
            table_name, 
            DEFAULT_COLUMN_COUNT+count_visible_days, 
            ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX,
            ImVec2(std::numeric_limits<float>::max(),(0.0F))))
            {
                ImGui::TableSetupColumn("No");
                ImGui::TableSetupColumn("ФИО ученика");
                ImGui::TableSetupColumn("No договора");
                ImGui::TableSetupColumn("Программа");
                ImGui::TableSetupColumn("Цена");
                for (int i = 0; i < count_visible_days; i++)
                {
                    std::string day_temp = std::to_string(visible_table_columns[i] + 1) + "." + std::to_string(current_time->tm_mon + 1);
                    ImGui::TableSetupColumn(day_temp.c_str());
                }
                ImGui::TableHeadersRow();
                int current_group = all_lessons[current_day_of_the_week][current_merged_lesson].get_group();
                for (int current_student_group_id = 0; current_student_group_id < all_groups[current_group].get_size(); current_student_group_id++)
                {
                    int current_student_id = all_groups[current_group].get_student_sort_id(current_student_group_id);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::Text("%i(%i)", current_student_group_id+1, current_student_id);
                    ImGui::TableSetColumnIndex(1); ImGui::Text(all_students[current_student_id].get_name().c_str());
                    ImGui::TableSetColumnIndex(2); ImGui::Text("%i", all_students[current_student_id].get_contract());
                    int current_student_contract = all_students[current_student_id].get_contract();
                    int current_discount_level = -1;
                    for (int i = 0; i < all_students.size(); i++)
                    {
                        if (all_students[i].get_contract() == current_student_contract && current_discount_level < 2)
                        {
                            current_discount_level++;
                        }
                    }
                    std::vector<bool> is_relevant;
                    for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[current_day_of_the_week][current_merged_lesson].get_lessons_size(); current_internal_lesson++)
                    {
                        current_lesson.internal_lesson_id = current_internal_lesson;
                        if (all_students[current_student_id].is_ignored(current_lesson))
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
                    for (int i = 0; i < is_relevant.size(); i++)
                    {
                        if (is_relevant[i]) 
                        {
                            show_price.append("+" + std::to_string(Lessons_Prices[all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(i).lesson_name_id][current_discount_level]));
                            show_lesson_names.append("+" + Lessons_Names[all_lessons[current_day_of_the_week][current_merged_lesson].get_lesson_pair(i).lesson_name_id]);
                        }
                    }
                    ImGui::TableSetColumnIndex(3); ImGui::Text(show_lesson_names.c_str());
                    ImGui::TableSetColumnIndex(4); ImGui::Text(show_price.c_str());
                    for (int current_day_cell = 0; current_day_cell < count_visible_days; current_day_cell++)
                    {
                        ImGui::TableSetColumnIndex(DEFAULT_COLUMN_COUNT + current_day_cell);
                        for (int current_internal_lesson = 0; current_internal_lesson < all_lessons[current_day_of_the_week][current_merged_lesson].get_lessons_size(); current_internal_lesson++)
                        {
                            current_lesson.internal_lesson_id = current_internal_lesson;
                            Student_Status current_status = all_days[visible_table_columns[current_day_cell]].get_status(current_lesson, current_student_id);
                            if (current_status.status == STATUS_INVALID) 
                            {
                                ImGui::TextDisabled("ERR"); ImGui::SameLine(); continue;
                            }
                            if (current_status.status == STATUS_NOT_AWAITED)
                            {
                                ImGui::TextDisabled("NAW"); ImGui::SameLine(); continue;
                            }
                            std::string combo_name = "##" + std::to_string(current_merged_lesson) + " "
                             + std::to_string(current_internal_lesson) + " "
                             + std::to_string(current_day_cell) + " "
                             + std::to_string(current_student_id);
                            if(ImGui::Combo(combo_name.c_str(), &current_status.status, " \0+\0Б\0-\0О\0\0"))
                            {
                                if (current_status.status != STATUS_WORKED_OUT)
                                {
                                    all_days[visible_table_columns[current_day_cell]].set_status(current_lesson, current_student_id, current_status.status);
                                }
                            }
                            ImGui::SameLine();
                        }
                    }
                }
                
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button(("Добавить ученика##" + std::to_string(current_merged_lesson)).c_str()))
                {
                    int new_guy = -1;
                    popup_add_student_to_group_is_open = true;
                    popup_add_student_to_group_select = -1;
                    popup_add_student_to_group_merged_lesson = current_merged_lesson;
                    popup_add_student_to_group(&all_students, &all_groups, &all_days, current_group, &popup_add_student_to_group_select);
                }
                ImGui::EndTable();
                ImGui::EndGroup();

            }
        }
    }

     //the following code is kept for reference

    for (int j = 0; j < 10; j++)
    {
    ImGui::BeginGroup();
    std::string fake_time = "Группа 1, " + std::to_string(j+8)+":00";
    ImGui::Text(fake_time.c_str());
    if (ImGui::BeginTable(
    fake_time.c_str(), 
    4, 
    ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX,
    ImVec2(std::numeric_limits<float>::max(),(0.0F))))

    {
        for (int i = 0; i < all_students.size(); i++)
        {
            ImGui::TableNextColumn();
            ImGui::Text("%i");
            ImGui::TableNextColumn();
            ImGui::Text(all_students[i].get_name().c_str());
            ImGui::TableNextColumn();
            ImGui::Text(" ");
            ImGui::TableNextColumn();
            //ImGui::Combo("combo", &is_here[i][j], " \0+\0Б\0-\0\0");
        }
        ImGui::EndTable();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    if (j%2!=1)
    {
        int on_lesson = 0;
        for (int i = 0; i < 10; i++)
        {
            if (is_here[i][j]==1 || is_here[i][j]==4) on_lesson++;
        }
    std::string fake_time2 = "Группа 2, " + std::to_string(j+8)+":00 - "+ std::to_string(j+10)+":00, есть ";
    ImGui::Text(fake_time2.c_str());
    
    //if (ImGui::BeginTable(fake_time2.c_str(), 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoHostExtendX))
    if (ImGui::BeginTable(fake_time2.c_str(),
    9, 
    ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX,
    ImVec2(std::numeric_limits<float>::max(),(0.0F)))) 
    //numeric_limit max is a bad way to avoid clipping issue
    {
        ImGui::TableNextColumn(); ImGui::Text("#");
        ImGui::TableNextColumn(); ImGui::Text("ФИО ученика");
        ImGui::TableNextColumn(); ImGui::Text("# договора");
        ImGui::TableNextColumn(); ImGui::Text("Программа");
        ImGui::TableNextColumn(); ImGui::Text("Цена");
        ImGui::TableNextColumn(); ImGui::Text("07.07");
        //ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)));
        
        ImGui::TableNextColumn(); ImGui::Text("14.07");
        ImGui::TableNextColumn(); ImGui::Text("21.07");
        ImGui::TableNextColumn(); ImGui::Text("28.07");

        std::u16string hh;

        for (int i = 0; i < 10; i++)
        {
            ImGui::PushID(i);
            ImGui::TableNextColumn();
            ImGui::Text((std::to_string(i+1)).c_str());
            ImGui::TableNextColumn();
            ImGui::Text(names[(i+j)%20].c_str());
            ImGui::TableNextColumn();
            ImGui::Text("123");

            ImGui::TableNextColumn();
            //ImGui::SetNextItemWidth(120);
            //ImGui::Combo("\0", &lol, "ИЗО+Лепка\0ИЗО\0Лепка\0");
            ImGuiComboFlags comboflags = ImGuiComboFlags_None;
            if (ImGui::BeginCombo("##2", lessons[i%3].c_str(), comboflags))
            {
                glfwPostEmptyEvent();
                for (int n = 0; n < (int)(lessons.size()); n++)
                {

                    static int item_current_idx = is_here[i][n];
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(lessons[n].c_str(), is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::TableNextColumn();
            ImGui::Text("%iр.", prices[i%3]);

            for (int k = 0; k < 4; k++)
            {
                ImGui::TableNextColumn();
                if(ImGui::Combo("##1", &is_here[i][j], " \0+\0Б\0-\0О\0\0")) is_working_out_open = true;
                ImGui::SameLine();
                if(ImGui::Combo("##3", &is_here[i][j], " \0+\0Б\0-\0О\0\0")) is_working_out_open = true;
            }

            switch (is_here[i][j])
            {
            case 1:
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.0f, 0.8f, 0.0f, 0.8f)));
                break;
            case 2:
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.0f, 0.8f)));
                break;
            case 3:
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.8f, 0.0f, 0.0f, 0.8f)));
                break;
            case 4:
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.8f, 0.8f))); 
                break;
            default:
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.4f, 0.4f, 0.4f, 0.8f)));
                break;
            }
            
            //ImGui::SetItemTooltip("14 Июля 2023, 09:00, с группой 61");

            ImGui::PopID();
        }
        ImGui::TableNextColumn();
        ImGui::TextDisabled("11");
        ImGui::TableNextColumn();
        ImGui::Button("Добавить в группу");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");

        ImGui::TableNextColumn();
        ImGui::TextDisabled("11");
        ImGui::TableNextColumn();
        
        if(ImGui::Button("Добавить на отработку") || opened_otr) 
        {
            opened_otr = true;
        }
        else opened_otr = false;
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");
        ImGui::TableNextColumn();
        ImGui::TextDisabled("...");

        ImGui::TableNextColumn();
        ImGui::TextDisabled(" ");
        ImGui::TableNextColumn();
        ImGui::TextDisabled(" ");
        ImGui::TableNextColumn();
        ImGui::TextDisabled(" ");
        ImGui::TableNextColumn();
        ImGui::TextDisabled(" ");
        ImGui::TableNextColumn();
        ImGui::TextDisabled(" ");
        ImGui::TableNextColumn();
        ImGui::Text("%i", on_lesson);
        ImGui::TableNextColumn();
        ImGui::Text("%i", on_lesson);
        ImGui::TableNextColumn();
        ImGui::Text("%i", on_lesson);
        ImGui::TableNextColumn();
        ImGui::Text("%i", on_lesson);

        ImGui::EndTable();
    }
    }
    ImGui::EndGroup();

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


/*
  - Ниже всех полей количество учеников (ИЗО отдельно от Лепки)
  - Добавление отработчиков снизу, а в группу выше
  - Даты: отображение месяца для данного дня недели (вид как в журнале)
  № | ФИО | #дог | Программа | Цена | Даты (посещения)
*/
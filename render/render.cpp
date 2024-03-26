#include "render.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Render::main_loop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        //TODO: filter events
        show_frame();
    }
}

void Render::prepare_first_frame()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        throw std::invalid_argument("GLFW: cannot init");
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

    window = glfwCreateWindow(1280, 720, "Журнал версии 0.0.1", nullptr, nullptr);
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
    if (window == nullptr)
        throw std::invalid_argument("GLFW: cannot create window");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    #ifdef _WIN32
    if(!io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic()))
    {
        throw std::invalid_argument("TTF: cannot find segoeni.ttf");
    }
    #else
    if(!io->Fonts->AddFontFromFileTTF("segoeui.ttf", 18.0f, nullptr, io->Fonts->GetGlyphRangesCyrillic()))
    {
        throw std::invalid_argument("TTF: cannot find segoeni.ttf");
    }
    #endif //not supporting apple platform

    //TODO: load here
    std::ifstream ifs("month.save");
    if (!ifs.fail())
    {
        boost::archive::text_iarchive ia(ifs);
        // write class instance to archive
        ia >> all_students;
        ia >> all_groups;
        ia >> all_lessons;
        ia >> all_days;
        // archive and stream closed when destructors are called
    }
}

void Render::show_frame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("Журнал версии 0.1.0", nullptr, WINDOW_FLAGS);

    Render::show_subwindows();
    Render::show_popups();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None | ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    if(ImGui::Button("Изменить день"))
    {
        Graphical::popup_select_day_of_the_week = new Popup_Select_Day_Of_The_Week();
    };
    ImGui::SameLine();
    if (ImGui::Button("Добавить группу"))
        Graphical::popup_add_merged_lesson_to_journal = new Popup_Add_Merged_Lesson_To_Journal(Graphical::wday());
    ImGui::SameLine();
    if (ImGui::Button("Ученики") )
        Graphical::subwindow_students_list = new Subwindow_Students_List();
    ImGui::SameLine();
    if (ImGui::Button("Группы"))
        Graphical::subwindow_lessons_list = new Subwindow_Lessons_List();
    ImGui::SameLine();
    ImGui::Button("Журнал оплаты");
    ImGui::SameLine();
    ImGui::Button("Справка");
    ImGui::SameLine();
    bool edit_mode = Graphical::is_edit_mode();
    if (ImGui::Checkbox("Режим редактирования", &edit_mode))
        Graphical::set_edit_mode(edit_mode);
    ImGui::Text("Выбран день %s, %s текущего года", Journal::Wday_name(Graphical::wday()), Journal::Month_name(Journal::current_month()));
    ImGui::BeginChild("Child", ImVec2(0, -20), true, window_flags);
    if (Journal::lesson_info_count(Graphical::wday()) == 0) 
        ImGui::Text("На текущий день не запланированы уроки.");
    for (int merged_lesson_id = 0; merged_lesson_id < Journal::lesson_info_count(Graphical::wday()); merged_lesson_id++)
    {
        Elements::table(merged_lesson_id);
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::End();
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void Render::prepare_shutdown()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    {
        std::ofstream ofs("month.save");
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << all_students;
        oa << all_groups;
        oa << all_lessons;
        oa << all_days;
    	// archive and stream closed when destructors are called
    }
}

#include "render.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Render::change_current_month(int month, int year)
{
    bool is_current_month_main = journal_main == journal;
    bool is_changed_month_main = journal_main->current_month() == month && journal_main->current_year() == year;
    if (is_changed_month_main && is_current_month_main) return;

    int backup_wday = graphical->wday;
    if (is_current_month_main)
    {
        journal->save();
    }
    else
    {
        delete journal;
        delete graphical;
    }

    // Data saved. Now load everything for another month

    if (is_changed_month_main)
    {
        // Months with State::Preview can generate workouts that affect main journal.
        journal_main->load_workouts();
        journal = journal_main;
        graphical = graphical_main;
    }
    else
    {
        journal = new Journal(month, year, journal_main);
        graphical = new Graphical(PTRREF(journal));
    }

    graphical->select_wday(backup_wday);
    graphical->mainwindow = &mainwindow;
    graphical->mainwindow->update_graphical(graphical);
}

Render::Render(Journal* _journal, Graphical *_graphical)
 : journal(_journal), graphical(_graphical), mainwindow(graphical)
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

    window = glfwCreateWindow(1280, 720, "Журнал версии 0.1.0", nullptr, nullptr);
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
    if(!io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f, nullptr, io->Fonts->GetGlyphRangesCyrillic()))
    {
        throw std::invalid_argument("TTF: cannot find segoeni.ttf");
    }
    #else
    if(!io->Fonts->AddFontFromFileTTF("segoeui.ttf", 18.0f, nullptr, io->Fonts->GetGlyphRangesCyrillic()))
    {
        throw std::invalid_argument("TTF: cannot find segoeni.ttf");
    }
    #endif //not supporting apple platform

    graphical_main = graphical;
    journal_main = journal;

    graphical->mainwindow = &mainwindow;
}

void Render::main_loop()
{
    int save_counter = 0;
    const int save_every_n_seconds = 300;
    while (!glfwWindowShouldClose(window))
    {
        if (save_counter != (int)ImGui::GetTime() / save_every_n_seconds)
        {
            save_counter = (int)ImGui::GetTime() / save_every_n_seconds;
            journal->save();
            printf("Automatic save at %f seconds.\n", ImGui::GetTime());
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) set_poll_time(1);
        if (io->AnyKeyPressed) set_poll_time(0.2);
        if (poll_until >= ImGui::GetTime())
        {
            glfwWaitEventsTimeout(0.05f);
        }
        else
        {
            glfwWaitEvents();
        }
        show_frame();
    }
}

// the program usually doesn't update the screen by itself
// but if poll_until > GetTime() it updates at some frequency
void Render::set_poll_time(int active_s)
{
    if (ImGui::GetTime() + active_s < poll_until) return;
    poll_until = ImGui::GetTime() + active_s;
}

void Render::show_frame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    graphical->mainwindow->show_frame();
    Mainwindow::Callback callback = graphical->mainwindow->get_callback();
    if (callback == Mainwindow::Callback::month_left)
    {
        int month = journal->current_month();
        int year = journal->current_year();
        previous_month_for(month, year);
        change_current_month(month, year);
    }
    else if (callback == Mainwindow::Callback::month_right)
    {
        int month = journal->current_month();
        int year = journal->current_year();
        next_month_for(month, year);
        change_current_month(month, year);
    } else if (callback == Mainwindow::Callback::month_default)
    {
        change_current_month(journal_main->current_month(), journal_main->current_year());
    }
    show_subwindows();
    show_popups();

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
}


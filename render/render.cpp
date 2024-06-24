#include "render.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
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

    window = glfwCreateWindow(1280, 720, "Журнал версии 0.1.0", NULL, NULL);
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

    graphical->mainwindow = &mainwindow;
}

void Render::main_loop()
{
    while (!glfwWindowShouldClose(window))
    {
        if (poll_time >= ImGui::GetTime())
        {
            glfwPollEvents();
        }
        else
        {
            //glfwWaitEvents();
            glfwPollEvents();
            //bool should_update = io->MouseDelta.x && io->MouseDelta.y;
            //should_update |= ImGui::IsMouseDown(ImGuiMouseButton_Left);
            //should_update |= ImGui::IsMouseDown(ImGuiMouseButton_Middle);
            //should_update |= ImGui::IsMouseDown(ImGuiMouseButton_Right);
            //if (!should_update) continue;
        }
        show_frame();
    }
}

void Render::set_update_time(int ms)
{
    if (poll_time > ms) return;
    poll_time = ms + ImGui::GetTime();
}

void Render::show_frame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    graphical->mainwindow->show_frame();
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


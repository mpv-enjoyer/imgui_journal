#include "platforms.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static GLFWwindow* window = nullptr;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool impl::begin_init_renderer()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        impl::modern_platform_failed();
        return impl_legacy::begin_init_renderer();
    }
        //throw std::invalid_argument("GLFW: cannot init");
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
    glfwWindowHint(GLFW_POSITION_X, 10);
    glfwWindowHint(GLFW_POSITION_Y, 50);
    window = glfwCreateWindow(700, 500, WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr)
    {
        impl::modern_platform_failed();
        return impl_legacy::begin_init_renderer();
    }
        //throw std::invalid_argument("GLFW: cannot create window");
    glfwSetWindowSizeLimits(window, 700, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
	//set_window_titlebar_icon(window);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    return true;
}

void impl::begin_frame()
{
    if (impl::is_modern_platform_failed()) return impl_legacy::begin_frame();
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void impl::render_frame()
{
    if (impl::is_modern_platform_failed()) return impl_legacy::render_frame();
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

bool impl::should_close()
{
    if (impl::is_modern_platform_failed()) return impl_legacy::should_close();
    return glfwWindowShouldClose(window);
}

void impl::cleanup()
{
    if (impl::is_modern_platform_failed()) return impl_legacy::cleanup();
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

bool impl::is_mouse_button_pressed()
{
    if (impl::is_modern_platform_failed()) return impl_legacy::is_mouse_button_pressed();
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void impl::wait_events_timeout(double time)
{
    if (impl::is_modern_platform_failed()) return impl_legacy::wait_events_timeout(time);
    glfwWaitEventsTimeout(time);
}

void impl::wait_events()
{
    if (impl::is_modern_platform_failed()) return impl_legacy::wait_events();
    glfwWaitEvents();
}
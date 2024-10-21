#pragma once

#include <imgui.h>

#define WINDOW_NAME 		   "Журнал посещения"
#define LWINDOW_NAME (LPCWSTR)L"Журнал посещения"
#define WINDOW_MIN_WIDTH 800
#define WINDOW_MIN_HEIGHT 500

namespace impl_legacy
{
    bool begin_init_renderer();
    void begin_frame();
    void render_frame();
    bool should_close();
    void cleanup();
    bool is_mouse_button_pressed();
    void wait_events_timeout(double time);
    void wait_events();
}

namespace impl
{
    bool is_application_already_running();
    bool begin_init_renderer();
    bool load_font(ImGuiIO* io);
    bool is_modern_platform_failed();
    void modern_platform_failed();
    void begin_frame();
    void render_frame();
    bool should_close();
    void cleanup();
    bool is_mouse_button_pressed();
    void wait_events_timeout(double time);
    void wait_events();
    // void set_window_titlebar_icon(GLFWwindow* window);
}
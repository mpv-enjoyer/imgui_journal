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
    bool load_font(ImGuiIO* io);
    bool is_modern_platform_failed();
    void modern_platform_failed();
    bool begin_init_renderer();
    void begin_frame();
    void render_frame();
    bool should_close();
    void cleanup();
    bool is_mouse_button_pressed();
    void wait_events_timeout(double time);
    void wait_events();
    // void set_window_titlebar_icon(GLFWwindow* window);
}

#include <iostream>
#include <mutex>
using namespace std;

class Impl {
public:
    class Renderer
    {
    public:
        virtual bool is_initialized() = 0;
        virtual void begin_frame() = 0;
        virtual void render_frame() = 0;
        virtual bool should_close() = 0;
        virtual void cleanup() = 0;
        virtual bool is_mouse_button_pressed() = 0;
        virtual void wait_events_timeout(double time) = 0;
        virtual void wait_events() = 0;
        virtual bool is_modern() = 0;
        virtual const char* name() = 0;
        // void set_window_titlebar_icon(GLFWwindow* window);
    };
    class Platform
    {
    public:
        virtual bool is_application_already_running() = 0;
        virtual bool load_font(ImGuiIO* io) = 0;
        virtual const char* name() = 0;
    };
private:
    class CompiledPlatform : public Platform
    {
    public:
        CompiledPlatform() { };
        bool is_application_already_running() override;
        bool load_font(ImGuiIO* io) override;
        const char* name() override;
    };
    Renderer* _renderer = nullptr;
    CompiledPlatform _platform;
    static Impl* _instance_ptr;
    Impl() {}
public:
    Impl(const Impl& obj) = delete;
    static Impl* get_instance() {
        if (_instance_ptr == nullptr) _instance_ptr = new Impl();
        return _instance_ptr;
    }
    Renderer* renderer();
    Platform* platform();
    Renderer* force_set_renderer(Renderer* renderer);
};

namespace ImplOS
{
    bool is_application_already_running();
    bool load_font(ImGuiIO* io);
};
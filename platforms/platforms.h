#pragma once

#include <imgui.h>

#define WINDOW_NAME 		   "Журнал посещения"
#define LWINDOW_NAME (LPCWSTR)L"Журнал посещения"
#define WINDOW_MIN_WIDTH 800
#define WINDOW_MIN_HEIGHT 500

#include <array>

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
        virtual bool supports_images() = 0;
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
    enum class Renderers
    {
        SDL2,
        GLFW3
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
    std::array<Renderers, 2> _attempt_order = { Renderers::GLFW3, Renderers::SDL2 };
    Renderer* _renderer = nullptr;
    CompiledPlatform _platform;
    static Impl* _instance_ptr;
    Impl() {}
public:
    Impl(const Impl& obj) = delete;
    static Impl* instance()
    {
        if (_instance_ptr == nullptr) _instance_ptr = new Impl();
        return _instance_ptr;
    }
    Renderer* get_renderer();
    Platform* get_platform();
    void set_renderer(Renderers renderers);
    
    static Renderer* renderer();
    static Platform* platform();
    static void prefer_renderer(Renderers renderers);
};
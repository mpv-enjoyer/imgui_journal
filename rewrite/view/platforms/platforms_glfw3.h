#pragma once
#include "platforms.h"
#include "GLFW/glfw3.h"

class GLFW3_Renderer : public Impl::Renderer
{
    static void glfw_error_callback(int error, const char* description);
    bool init = false;
    GLFWwindow* window = nullptr;
public:
    GLFW3_Renderer();
    bool is_initialized() override;
    bool begin_frame(bool cancellable) override;
    void end_frame() override;
    bool should_close() override;
    void cleanup() override;
    bool is_mouse_button_pressed() override;
    void wait_events_timeout(double time) override;
    void wait_events() override;
    bool supports_images() override;
    const char* name() override;
};

class GLFW3_Initializer : public Impl::Renderer::Initializer
{
public:
    const char* name() override { return "GLFW3_OPENGL3_Renderer"; }
    Impl::Renderer* initialize() override { return new GLFW3_Renderer(); }
};
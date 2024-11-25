#pragma once
#include "platforms.h"

class SDL2_Renderer : public Impl::Renderer
{
    bool init = false;
public:
    SDL2_Renderer();
    bool is_initialized() override;
    void begin_frame() override;
    void render_frame() override;
    bool should_close() override;
    void cleanup() override;
    bool is_mouse_button_pressed() override;
    void wait_events_timeout(double time) override;
    void wait_events() override;
    bool is_modern() override;
    const char* name() override;
};
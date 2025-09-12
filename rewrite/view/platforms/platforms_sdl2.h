#pragma once
#include "platforms.h"
#include <SDL2/SDL.h>

class SDL2_Renderer : public Impl::Renderer
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool done = false;
    bool init = false;
public:
    SDL2_Renderer();
    bool is_initialized() override;
    void begin_frame() override;
    void end_frame() override;
    bool should_close() override;
    void cleanup() override;
    bool is_mouse_button_pressed() override;
    void wait_events_timeout(double time) override;
    void wait_events() override;
    bool supports_images() override;
    const char* name() override;
};
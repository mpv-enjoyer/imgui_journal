#include "platforms_sdl2.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

SDL2_Renderer::SDL2_Renderer()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(WINDOW_NAME, 10, 50, WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT, window_flags);
    SDL_SetWindowMinimumSize(window, WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    init = true;
}

bool SDL2_Renderer::is_initialized()
{
    return init;
}

bool SDL2_Renderer::begin_frame()
{
    static bool cancellable = false;
    const static bool LOG = false;
    
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    if (!ImGui_ImplSDL2_GetAndClearUncancellableEvents() && cancellable && ImGui::NewFrameMustBeCancelled())
    {
        ImGui_ImplSDL2_CancelFrame();
        return false;
    }
    cancellable = true;
    if (LOG) printf("Submitted \n");
    ImGui::NewFrame();
    return true;
}

void SDL2_Renderer::render_frame()
{
    ImGuiIO& io = ImGui::GetIO();
    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, (Uint8)(255), (Uint8)(255), (Uint8)(255), (Uint8)(255));
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

bool SDL2_Renderer::should_close()
{
    return done;
}

void SDL2_Renderer::cleanup()
{
    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool SDL2_Renderer::is_mouse_button_pressed()
{
    return SDL_GetMouseState(NULL, NULL) & SDL_MOUSEBUTTONUP;
}

double SDL2_Renderer::get_current_time()
{
    // repeat the logic from ImGui_ImplSDL2_NewFrame
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 current_time_uint64 = SDL_GetPerformanceCounter();
    return (double)(current_time_uint64) / frequency;
}

void SDL2_Renderer::wait_events_until(double time)
{
    double current_time = get_current_time();
    double previous_frame_time = ImGui::GetTime();
    double delta_time = previous_frame_time > 0.0 ? (float)(current_time - previous_frame_time) : (float)(1.0f / 60.0f); // TODO: we lost some presicion here.
    if (current_time < time)
    {
        wait_events_timeout(delta_time);
    }
    else
    {
        /* not an error - may happen */
    }
}

void SDL2_Renderer::wait_events_timeout(double time)
{
    SDL_Event event;
    SDL_WaitEventTimeout(&event, time * 1000);
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
        done = true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
        done = true;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }
}

void SDL2_Renderer::wait_events()
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
        done = true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
        done = true;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }
}

void SDL2_Renderer::poll_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }

}

bool SDL2_Renderer::supports_images()
{
    return false;
}

const char *SDL2_Renderer::name()
{
    return "SDL2/SDL2_Renderer";
}

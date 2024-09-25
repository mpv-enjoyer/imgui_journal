#include "platforms.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL2/SDL.h>

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static bool done = false;

bool impl_legacy::begin_init_renderer()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return false;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 500, window_flags);
    SDL_SetWindowMinimumSize(window, 700, 500);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return false;
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

    return true;
}

void impl_legacy::begin_frame()
{
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void impl_legacy::render_frame()
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

bool impl_legacy::should_close()
{
    return done;
}

void impl_legacy::cleanup()
{
    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool impl_legacy::is_mouse_button_pressed()
{
    return SDL_GetMouseState(NULL, NULL) & SDL_MOUSEBUTTONUP;
}

void impl_legacy::wait_events_timeout(double time)
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

void impl_legacy::wait_events()
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
#include "view/view.h"
#include "thirdparty/argh.h"

int main(int argc, char** argv)
{
    argh::parser cmdl(argc, argv);
    for (auto& param : cmdl.params())
    {
        if (param.first == "prefer")
        {
            Impl::prefer_renderer(param.second.c_str());
        }
    }
    View::View().main_loop();
}

/// ALL CPP FILES MOVED HERE FOR FASTER RECOMPILATION ///

/*
BEFORE:
real    0m6,767s
user    0m6,406s
sys     0m0,323s

AFTER:
real    0m3,902s
user    0m3,699s
sys     0m0,185s
*/

/*
This project is somewhat experimental for me as I tried
to not use any incremental build system, all the logic is
located in the header files. So why not take it to the
extreme and not place all the .cpp files right here so
I only have a single compilation unit.

The code in this file mostly consists of singletons and
there are also some platform-specific code. Thus there's
not a lot of code to maintain.
*/


// controller/controller_impl.h
#include "model/model_impl.h"
#include "controller/controller_impl.h"
#include "common/logs.h"
#include <iostream>

const IModel &Controller_Impl::model() const
{
    return Model_Impl::get();
}

void Controller_Impl::add(std::shared_ptr<ICommand> command)
{
    m_pending_commands.push(std::move(command));
}

void Controller_Impl::flush(Time_State::Bits view_time_state)
{
    while (!m_pending_commands.empty())
    {
        auto command = m_pending_commands.front();
        if (!command->is_fitting(view_time_state))
        {
            log_error("is_fitting violation, skipping command!");
        }
        auto error = command->get_error(model());
        if (!error) command->call(Model_Impl::get());
        else log_error(*error);
        m_pending_commands.pop();
        Model_Impl::get()->cache_invalidate();
    }
}

// model/model_impl.cpp
#include "model/model_impl.h"

Model_Impl* Model_Impl::m_instance_ptr = nullptr;


// view/platforms/platforms_glfw3.cpp
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "view/platforms/platforms_glfw3.h"

void GLFW3_Renderer::glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

GLFW3_Renderer::GLFW3_Renderer()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return;

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
    window = glfwCreateWindow(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr) return;

    glfwSetWindowSizeLimits(window, WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
	//set_window_titlebar_icon(window);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    init = setup_end();
}

bool GLFW3_Renderer::is_initialized()
{
    return init;
}

bool GLFW3_Renderer::begin_frame(bool cancellable)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    bool TEMP_BACKUP = ImGui::GetIO().AnyKeyPressed;
    ImGui_ImplGlfw_NewFrame();
    bool TEMP_BACKUP_2 = ImGui::GetIO().AnyKeyPressed;
    ImGui::GetIO().AnyKeyPressed = TEMP_BACKUP;
    double x, y;
    ImGui_ImplGlfw_GetCursorPosBeforeImGuiFrame(&x, &y);
    if (!ImGui_ImplGlfw_GetAndClearUncancellableEvents() && cancellable && ImGui::NewFrameMustBeCancelled(x, y)) 
    {
        ImGui_ImplGlfw_CancelFrame();
        return false;
    }
    printf("Submitted \n");
    ImGui::GetIO().AnyKeyPressed = TEMP_BACKUP_2;
    ImGui::NewFrame();
    return true;
}

void GLFW3_Renderer::end_frame()
{
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

bool GLFW3_Renderer::should_close()
{
    return glfwWindowShouldClose(window);
}

void GLFW3_Renderer::cleanup()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

bool GLFW3_Renderer::is_mouse_button_pressed()
{
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void GLFW3_Renderer::wait_events_timeout(double time)
{
    glfwWaitEventsTimeout(time);
}

void GLFW3_Renderer::wait_events()
{
    glfwWaitEvents();
}

void GLFW3_Renderer::poll_events()
{
    // For debug
    glfwPollEvents();
}

bool GLFW3_Renderer::supports_images()
{
    return true;
}

const char *GLFW3_Renderer::name()
{
    return GLFW3_Initializer().name();
}

// view/platforms/platforms_sdl2.cpp
#include "view/platforms/platforms_sdl2.h"
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
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    init = setup_end();
}

bool SDL2_Renderer::is_initialized()
{
    return init;
}

bool SDL2_Renderer::begin_frame(bool cancellable)
{
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    return true; // TODO
}

void SDL2_Renderer::end_frame()
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
    return SDL2_Initializer().name();
}


// view/platforms/platforms.cpp
#include "view/platforms/platforms.h"
#include <stdio.h>
#include "view/platforms/platforms_glfw3.h"
#include "view/platforms/platforms_sdl2.h"

Impl* Impl::_instance_ptr = nullptr;

Impl::Impl()
{
    _initializers =
    {
        new GLFW3_Initializer(),
        new SDL2_Initializer()
    };
}

Impl::Renderer *Impl::get_renderer()
{
    if (_renderer == nullptr)
    {
        for (auto* initializer : _initializers)
        {
            _renderer = initializer->initialize();
            if (_renderer->is_initialized()) return _renderer;
            printf("Cannot initialize %s renderer.\n", _renderer->name());
        }
        _renderer = nullptr;
        IM_ASSERT(false && "Cannot initialize graphics!");
    }
    return _renderer;
};

Impl::Platform* Impl::get_platform()
{
    return &_platform;
}

auto Impl::renderer() -> Renderer*
{
    return instance()->get_renderer();
}

auto Impl::platform() -> Platform*
{
    return instance()->get_platform();
}

void Impl::prefer_renderer(const char *renderer)
{
    IM_ASSERT(!instance()->_renderer);

    for (auto it = instance()->_initializers.begin(); it != instance()->_initializers.end(); ++it)
    {
        Renderer::Initializer* initializer = *it;
        if (std::string(initializer->name()) == std::string(renderer))
        {
            std::iter_swap(instance()->_initializers.begin(), it);
            return;
        }
    }
    printf("Cannot prefer renderer %s: not found\n", renderer);
}
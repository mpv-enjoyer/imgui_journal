#include "platforms.h"
#include <stdio.h>
#include "platforms_glfw3.h"
#include "platforms_sdl2.h"

Impl::Renderer* Impl::renderer()
{
    if (_renderer == nullptr)
    {
        _renderer = new GLFW3_Renderer();
        if (_renderer->is_initialized()) return _renderer;
        printf("Cannot initialize %s renderer. Fall back.\n", _renderer->name());
        _renderer = new SDL2_Renderer();
        if (_renderer->is_initialized()) return _renderer;
        _renderer = nullptr;
        IM_ASSERT(false && "Cannot initialize graphics!");
    }
    return _renderer;
};

Impl::Platform* Impl::platform()
{
    return &_platform;
}

auto Impl::force_set_renderer(Renderer *renderer) -> Renderer*
{
    IM_ASSERT(_renderer == nullptr);
    _renderer = renderer;
    printf("Force set %s renderer.\n", renderer->name());
}

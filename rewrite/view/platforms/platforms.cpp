#include "platforms.h"
#include <stdio.h>
#include "platforms_glfw3.h"
#include "platforms_sdl2.h"

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
    IM_ASSERT(!instance()->get_renderer());

    for (auto it = instance()->_initializers.begin(); it != instance()->_initializers.end(); ++it)
    {
        auto* initializer = *it;
        if (std::string(initializer->name()) == std::string(renderer))
        {
            std::iter_swap(instance()->_initializers.begin(), it);
            return;
        }
    }
    printf("Cannot prefer renderer %s: not found\n", renderer);
}
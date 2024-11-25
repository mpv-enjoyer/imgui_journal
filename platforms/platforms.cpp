#include "platforms.h"
#include <stdio.h>
#include "platforms_glfw3.h"
#include "platforms_sdl2.h"

Impl* Impl::_instance_ptr = nullptr;

Impl::Renderer* Impl::get_renderer()
{
    if (_renderer == nullptr)
    {
        for (auto option : _attempt_order)
        {
            switch (option)
            {
            case Renderers::GLFW3:
                _renderer = new GLFW3_Renderer();
                break;
            case Renderers::SDL2:
                _renderer = new SDL2_Renderer();
                break;
            default:
                IM_ASSERT("Broken renderer attempt order");
            }
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

void Impl::set_renderer(Renderers renderers)
{
    IM_ASSERT(_renderer == nullptr);
    for (std::size_t i = 1; i < _attempt_order.size(); i++)
    {
        if (_attempt_order[i] == renderers)
        {
            std::swap(_attempt_order[i], _attempt_order[0]);
        }
    }
}

auto Impl::renderer() -> Impl::Renderer*
{
    return instance()->get_renderer();
}

auto Impl::platform() -> Platform*
{
    return instance()->get_platform();
}

void Impl::prefer_renderer(Renderers renderers)
{
    instance()->set_renderer(renderers);
}
